# (C) 2026 see Authors.txt
#
# This file is part of MPC-Kelpie.
#
# MPC-Kelpie is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# MPC-Kelpie is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""Create the MPC-Kelpie release packages.

Run after "build.bat x64 Installer" with the MPC Video Renderer of the
release in distrib\\MPCVR. Writes to bin\\kelpie-release:

  MPC-Kelpie.<version>.x64.exe              installer (copied from bin)
  MPC-Kelpie.<version>.x64.zip              portable package
  MPC-Kelpie.<version>.source.zip           source code with submodules
  MPCVideoRenderer.<version>.source.zip     MPC Video Renderer source code
  SHA256SUMS.txt

The source archives are made from fresh clones of the given tags (or
branches) of the public repositories, without the .git data.
"""

import argparse
import fnmatch
import hashlib
import os
import shutil
import stat
import struct
import subprocess
import sys
import tempfile
import time
import zipfile

REPO_URL = "https://github.com/rami-bakhit/mpc-hc.git"
MPCVR_URL = "https://github.com/rami-bakhit/MPCVideoRenderer.git"

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
BIN_DIR = os.path.join(ROOT, "bin", "mpc-hc_x64")
DISTRIB_DIR = os.path.join(ROOT, "distrib")
OUT_DIR = os.path.join(ROOT, "bin", "kelpie-release")

EXE_NAME = "mpc-kelpie64.exe"
MPCVR_AX = "MpcVideoRenderer64.ax"


class PackageError(Exception):
    pass


def file_version(path):
    """Return the file version of a PE file from its VS_FIXEDFILEINFO."""
    with open(path, "rb") as f:
        data = f.read()
    pos = data.find(struct.pack("<I", 0xFEEF04BD))
    if pos < 0:
        raise PackageError("No version information in " + path)
    ms, ls = struct.unpack_from("<II", data, pos + 8)
    return "%d.%d.%d.%d" % (ms >> 16, ms & 0xFFFF, ls >> 16, ls & 0xFFFF)


def sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def require(path):
    if not os.path.isfile(path):
        raise PackageError("Missing file: " + path)
    return path


def glob_files(directory, *patterns):
    names = sorted(os.listdir(directory)) if os.path.isdir(directory) else []
    return [os.path.join(directory, n) for n in names
            if any(fnmatch.fnmatch(n.lower(), p.lower()) for p in patterns)
            and os.path.isfile(os.path.join(directory, n))]


def portable_files():
    """List (archive name, source path) pairs, from the same places as the installer."""
    files = [
        (EXE_NAME, require(os.path.join(BIN_DIR, EXE_NAME))),
        ("mpciconlib.dll", require(os.path.join(BIN_DIR, "mpciconlib.dll"))),
        ("MediaInfo.dll", require(os.path.join(DISTRIB_DIR, "x64", "MediaInfo.dll"))),
        ("D3DCompiler_47.dll", require(os.path.join(DISTRIB_DIR, "x64", "D3DCompiler_47.dll"))),
        ("D3DX9_43.dll", require(os.path.join(DISTRIB_DIR, "x64", "D3DX9_43.dll"))),
        ("COPYING.txt", require(os.path.join(ROOT, "COPYING.txt"))),
        ("MPCVR/" + MPCVR_AX, require(os.path.join(DISTRIB_DIR, "MPCVR", MPCVR_AX))),
    ]

    groups = [
        ("Lang", glob_files(os.path.join(BIN_DIR, "Lang"),
                            "mpcresources.??.dll", "mpcresources.??_??.dll")),
        ("LAVFilters64", glob_files(os.path.join(BIN_DIR, "LAVFilters64"),
                                    "*.dll", "*.ax", "*.manifest")),
        ("Shaders", glob_files(os.path.join(ROOT, "src", "mpc-hc", "res", "shaders", "dx9"), "*.hlsl")),
        ("Shaders11", glob_files(os.path.join(ROOT, "src", "mpc-hc", "res", "shaders", "dx11"), "*.hlsl")),
    ]
    for folder, paths in groups:
        if not paths:
            raise PackageError("No files for " + folder)
        files += [(folder + "/" + os.path.basename(p), p) for p in paths]

    toolbars = os.path.join(DISTRIB_DIR, "Toolbars")
    for dirpath, dirnames, filenames in os.walk(toolbars):
        dirnames.sort()
        for name in sorted(filenames):
            path = os.path.join(dirpath, name)
            rel = os.path.relpath(path, toolbars).replace(os.sep, "/")
            files.append(("Toolbars/" + rel, path))
    return files


def write_zip(zip_path, entries):
    tmp = zip_path + ".part"
    with zipfile.ZipFile(tmp, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as z:
        for name, path in entries:
            z.write(path, name)
    os.replace(tmp, zip_path)


def git(*args, env=None):
    subprocess.run(["git"] + list(args), check=True, env=env)


def git_output(*args, env=None):
    return subprocess.run(["git"] + list(args), check=True, env=env,
                          capture_output=True, text=True).stdout


def remove_tree(path):
    def make_writable(func, p, *_):
        os.chmod(p, stat.S_IWRITE)
        func(p)
    if sys.version_info >= (3, 12):
        shutil.rmtree(path, onexc=make_writable)
    else:
        shutil.rmtree(path, onerror=make_writable)


def source_archive(url, ref, top_dir, zip_path, work_dir):
    """Clone a tag or branch with its submodules and archive the tracked files.

    The archive does not depend on the computer: the git settings that change
    checked out files (line endings, symbolic links, attributes outside the
    repositories, hooks) are fixed for the clone, the file list, file modes
    and order come from git, and every entry has the commit time. The same
    commits give the same archive on Windows and Linux.

    The clone goes to a short directory: some submodule files have paths of
    almost 190 characters and Windows limits a path to 260 (MAX_PATH).
    """
    clone = os.path.join(work_dir, "c")
    attributes = os.path.join(work_dir, "attributes")
    hooks = os.path.join(work_dir, "hooks")
    open(attributes, "w").close()
    os.makedirs(hooks, exist_ok=True)
    env = dict(os.environ, GIT_ATTR_NOSYSTEM="1")
    settings = []
    for setting in ("core.autocrlf=false", "core.eol=lf", "core.symlinks=false",
                    "core.attributesFile=" + attributes.replace(os.sep, "/"),
                    "core.hooksPath=" + hooks.replace(os.sep, "/")):
        settings += ["-c", setting]

    print("Cloning %s (%s)..." % (url, ref))
    git(*settings, "clone", "--quiet", "--depth", "1", "--branch", ref,
        "--recurse-submodules", "--shallow-submodules", url, clone, env=env)
    status = git_output("-C", clone, "submodule", "status", "--recursive", env=env)
    bad = [line for line in status.splitlines() if line[:1] in ("-", "+", "U")]
    if bad:
        raise PackageError("Submodules not at the recorded commits:\n" + "\n".join(bad))
    commit = git_output("-C", clone, "rev-parse", "HEAD", env=env).strip()
    date_time = time.gmtime(int(git_output("-C", clone, "log", "-1", "--format=%ct", env=env)))[:6]

    files = git_output("-C", clone, "ls-files", "-z", "--stage", "--recurse-submodules", env=env)
    tmp = zip_path + ".part"
    count = 0
    with zipfile.ZipFile(tmp, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as z:
        for item in files.split("\0"):
            if not item:
                continue
            info, name = item.split("\t", 1)
            mode = int(info.split(" ", 1)[0], 8)
            if mode not in (0o100644, 0o100755, 0o120000):
                raise PackageError("Unexpected git mode %o: %s" % (mode, name))
            path = os.path.join(clone, *name.split("/"))
            if not os.path.isfile(path):
                raise PackageError("Missing in the checkout: " + name)
            with open(path, "rb") as f:
                data = f.read()
            entry = zipfile.ZipInfo(top_dir + "/" + name, date_time)
            entry.create_system = 3
            entry.external_attr = (mode | (0o777 if mode == 0o120000 else 0)) << 16
            z.writestr(entry, data, zipfile.ZIP_DEFLATED, 9)
            count += 1
    os.replace(tmp, zip_path)
    remove_tree(clone)
    return commit, count


def main():
    parser = argparse.ArgumentParser(description="Create the MPC-Kelpie release packages.")
    parser.add_argument("--ref", help="tag or branch of " + REPO_URL)
    parser.add_argument("--mpcvr-ref", help="tag or branch of " + MPCVR_URL)
    parser.add_argument("--no-sources", action="store_true",
                        help="skip the source archives (test packages only)")
    args = parser.parse_args()
    if not args.no_sources and not (args.ref and args.mpcvr_ref):
        parser.error("--ref and --mpcvr-ref are required unless --no-sources is given")

    exe = require(os.path.join(BIN_DIR, EXE_NAME))
    version = file_version(exe)
    ax = require(os.path.join(DISTRIB_DIR, "MPCVR", MPCVR_AX))
    mpcvr_version = file_version(ax)

    installer = require(os.path.join(ROOT, "bin", "MPC-Kelpie.%s.x64.exe" % version))
    installer_version = file_version(installer)
    if installer_version != version:
        raise PackageError("Installer version %s does not match %s %s"
                           % (installer_version, EXE_NAME, version))

    print("MPC-Kelpie %s" % version)
    print("  %-22s %s" % (EXE_NAME, sha256(exe)))
    print("  %-22s %s" % ("mpciconlib.dll", sha256(require(os.path.join(BIN_DIR, "mpciconlib.dll")))))
    print("  %-22s %s (%s)" % (MPCVR_AX, sha256(ax), mpcvr_version))

    os.makedirs(OUT_DIR, exist_ok=True)
    outputs = []

    name = "MPC-Kelpie.%s.x64.exe" % version
    shutil.copy2(installer, os.path.join(OUT_DIR, name))
    outputs.append(name)

    name = "MPC-Kelpie.%s.x64.zip" % version
    entries = portable_files()
    write_zip(os.path.join(OUT_DIR, name), entries)
    outputs.append(name)
    print("%s: %d files" % (name, len(entries)))

    if not args.no_sources:
        work_dir = tempfile.mkdtemp(prefix="src-", dir=OUT_DIR)
        try:
            for url, ref, top in ((REPO_URL, args.ref, "MPC-Kelpie.%s.source" % version),
                                  (MPCVR_URL, args.mpcvr_ref, "MPCVideoRenderer.%s.source" % mpcvr_version)):
                name = top + ".zip"
                commit, count = source_archive(url, ref, top, os.path.join(OUT_DIR, name), work_dir)
                outputs.append(name)
                print("%s: %d files, %s at %s" % (name, count, ref, commit))
        finally:
            remove_tree(work_dir)

    with open(os.path.join(OUT_DIR, "SHA256SUMS.txt"), "w", newline="\n") as f:
        for name in sorted(outputs):
            f.write("%s  %s\n" % (sha256(os.path.join(OUT_DIR, name)), name))
    print("SHA256SUMS.txt written to " + OUT_DIR)


if __name__ == "__main__":
    try:
        main()
    except (PackageError, subprocess.CalledProcessError) as e:
        sys.exit("ERROR: %s" % e)
