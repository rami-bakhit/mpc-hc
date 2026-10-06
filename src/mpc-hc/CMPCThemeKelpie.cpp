#include "stdafx.h"
#include "CMPCTheme.h"

//MPC-Kelpie: the MPC-Kelpie theme style. It is drawn by the Windows 11 style code (isWindows11Style) with its own
//palette: sea tinted surfaces with warm white text in dark mode, misty surfaces with sea navy text in light mode,
//a fixed turquoise accent instead of the Windows accent, and a seekbar in the same colours.
//InitializeKelpieColors follows CMPCTheme::InitializeWindows11Colors role by role, from the same Fluent tokens flattened
//over the Kelpie surfaces; when upstream changes the Windows 11 palette, the same change belongs here.

//as in CMPCTheme.cpp: token 0xAARRGGBB; per channel: s + (c - s) * a / 255, rounded
static COLORREF Flatten(DWORD token, COLORREF surface) {
    const int a = (token >> 24) & 0xFF;
    auto blend = [a](int c, int s) { return (BYTE)((s * 255 + (c - s) * a + 127) / 255); };
    return RGB(blend((token >> 16) & 0xFF, GetRValue(surface)), blend((token >> 8) & 0xFF, GetGValue(surface)), blend(token & 0xFF, GetBValue(surface)));
}

static COLORREF Opaque(DWORD token) {
    return RGB((token >> 16) & 0xFF, (token >> 8) & 0xFF, token & 0xFF);
}

//FluentDark with the Kelpie surfaces; text and focus strokes in the Kelpie warm white
namespace KelpieDark {
    constexpr DWORD TextFillColorPrimary = 0xFFEFE9DE;
    constexpr DWORD TextFillColorSecondary = 0xC5EFE9DE;
    constexpr DWORD TextFillColorTertiary = 0x87EFE9DE;
    constexpr DWORD TextFillColorDisabled = 0x5DEFE9DE;
    constexpr DWORD ControlFillColorDefault = 0x0FFFFFFF;
    constexpr DWORD ControlFillColorSecondary = 0x15FFFFFF;
    constexpr DWORD ControlFillColorTertiary = 0x08FFFFFF;
    constexpr DWORD ControlFillColorDisabled = 0x0BFFFFFF;
    constexpr DWORD ControlStrongFillColorDefault = 0x8BFFFFFF;
    constexpr DWORD ControlStrongFillColorDisabled = 0x3FFFFFFF;
    constexpr DWORD ControlSolidFillColorDefault = 0xFF304648;
    constexpr DWORD SubtleFillColorSecondary = 0x0FFFFFFF;
    constexpr DWORD SubtleFillColorTertiary = 0x0AFFFFFF;
    constexpr DWORD ControlAltFillColorSecondary = 0x19000000;
    constexpr DWORD ControlAltFillColorTertiary = 0x0BFFFFFF;
    constexpr DWORD ControlStrokeColorDefault = 0x12FFFFFF;
    constexpr DWORD ControlStrokeColorSecondary = 0x18FFFFFF;
    constexpr DWORD ControlStrokeColorOnAccentSecondary = 0x23000000;
    constexpr DWORD CardStrokeColorDefault = 0x19000000;
    constexpr DWORD ControlStrongStrokeColorDefault = 0x8BFFFFFF;
    constexpr DWORD ControlStrongStrokeColorDisabled = 0x28FFFFFF;
    constexpr DWORD AccentFillColorDisabled = 0x28FFFFFF;
    constexpr DWORD TextOnAccentFillColorDisabled = 0x87FFFFFF;
    constexpr DWORD SurfaceStrokeColorDefault = 0x66757575;
    constexpr DWORD SurfaceStrokeColorFlyout = 0x33000000;
    constexpr DWORD DividerStrokeColorDefault = 0x15FFFFFF;
    constexpr DWORD FocusStrokeColorOuter = 0xFFEFE9DE;
    constexpr DWORD LayerFillColorDefault = 0xFF162629;
    constexpr DWORD SolidBackgroundFillColorBase = 0xFF0F1C1E;
    constexpr DWORD SolidBackgroundFillColorSecondary = 0xFF0C1719;
    constexpr DWORD SolidBackgroundFillColorQuarternary = 0xFF1C2E31;
    constexpr DWORD SolidBackgroundFillColorBaseAlt = 0xFF081113;
    constexpr DWORD SystemFillColorCriticalBackground = 0xFF442726;
}

//FluentLight with the Kelpie surfaces; text in sea navy, the other text tokens in a sea tinted black ink
namespace KelpieLight {
    constexpr DWORD TextFillColorPrimary = 0xFF13272A;
    constexpr DWORD TextFillColorSecondary = 0x9E000F12;
    constexpr DWORD TextFillColorTertiary = 0x72000F12;
    constexpr DWORD TextFillColorDisabled = 0x5C000F12;
    constexpr DWORD TextOnAccentFillColorPrimary = 0xFFFFFFFF;
    constexpr DWORD ControlFillColorDefault = 0xB3FFFFFF;
    constexpr DWORD ControlFillColorSecondary = 0x80F9F9F9;
    constexpr DWORD ControlFillColorTertiary = 0x4DF9F9F9;
    constexpr DWORD ControlStrongFillColorDefault = 0x72000000;
    constexpr DWORD ControlStrongFillColorDisabled = 0x51000000;
    constexpr DWORD SubtleFillColorSecondary = 0x09000000;
    constexpr DWORD SubtleFillColorTertiary = 0x06000000;
    constexpr DWORD ControlAltFillColorSecondary = 0x06000000;
    constexpr DWORD ControlAltFillColorTertiary = 0x0F000000;
    constexpr DWORD ControlAltFillColorQuarternary = 0x18000000;
    constexpr DWORD ControlStrokeColorDefault = 0x0F000000;
    constexpr DWORD ControlStrokeColorSecondary = 0x29000000;
    constexpr DWORD ControlSolidFillColorDefault = 0xFFFFFFFF;
    constexpr DWORD ControlStrokeColorOnAccentSecondary = 0x66000000;
    constexpr DWORD CardStrokeColorDefault = 0x0F000000;
    constexpr DWORD ControlStrongStrokeColorDefault = 0x72000000;
    constexpr DWORD ControlStrongStrokeColorDisabled = 0x37000000;
    constexpr DWORD AccentFillColorDisabled = 0x37000000;
    constexpr DWORD TextOnAccentFillColorDisabled = 0xFFFFFFFF;
    constexpr DWORD SurfaceStrokeColorDefault = 0x66757575;
    constexpr DWORD SurfaceStrokeColorFlyout = 0x0F000000;
    constexpr DWORD DividerStrokeColorDefault = 0x0F000000;
    constexpr DWORD FocusStrokeColorOuter = 0xE4000F12;
    constexpr DWORD LayerFillColorDefault = 0xFFF3F7F7;
    constexpr DWORD SolidBackgroundFillColorBase = 0xFFE7EEEE;
    constexpr DWORD SolidBackgroundFillColorSecondary = 0xFFE1E9E9;
    constexpr DWORD SolidBackgroundFillColorTertiary = 0xFFFAFCFC;
    constexpr DWORD SolidBackgroundFillColorBaseAlt = 0xFFD3DEDE;
    constexpr DWORD SystemFillColorCritical = 0xFFC42B1C;
    constexpr DWORD SystemFillColorCriticalBackground = 0xFFFDE7E9;
}

//the MPC-Kelpie accent: the turquoise of the line under the logo in seven shades, AccentDark3 .. AccentLight3
void CMPCTheme::SetKelpieAccentColors() {
    AccentDark3 = RGB(16, 35, 35);
    AccentDark2 = RGB(26, 55, 55);
    AccentDark1 = RGB(39, 83, 83);
    Accent = RGB(54, 114, 114);
    AccentLight1 = RGB(73, 156, 156);
    AccentLight2 = RGB(118, 191, 191);
    AccentLight3 = RGB(165, 213, 213);
}

void CMPCTheme::InitializeKelpieColors() {
    InitializeWindows11Colors(); //every slot gets a value first, also one upstream adds later; the Kelpie palette then replaces them
    if (EffectiveThemeMode() == ModernThemeMode::DARK) {
        using namespace KelpieDark;
        drawThemedControls = true;
        //not a fluent token: fluent's control strokes nearly vanish without mica behind them, and even the surface stroke
        //left buttons, combo boxes and text boxes hard to make out, so their frames use this
        constexpr DWORD ControlStrokeColorVisible = 0x40FFFFFF;

        const COLORREF base = Opaque(SolidBackgroundFillColorBase);
        const COLORREF layer = Flatten(LayerFillColorDefault, base); //content surfaces sit one layer above the base

        MenuBGColor = Opaque(SolidBackgroundFillColorQuarternary);
        MenubarBGColor = base;
        WindowBGColor = base;
        ControlAreaBGColor = Opaque(SolidBackgroundFillColorSecondary);

        ContentBGColor = layer;
        PlaylistSelectedColor = Flatten(SubtleFillColorSecondary, ContentBGColor); //list selection is neutral, the accent goes on an indicator
        ContentSelectedColor = Opaque(ControlSolidFillColorDefault); //neutral selection as in explorer; only the light selection is the accent
        PlayerBGColor = layer; //player bars share the content layer so the seekbar stays flush with the toolbar

        MenuSelectedColor = Flatten(SubtleFillColorSecondary, MenuBGColor);
        MenubarSelectedBGColor = Flatten(SubtleFillColorSecondary, MenubarBGColor);
        MenuSeparatorColor = Flatten(DividerStrokeColorDefault, MenuBGColor);
        MenuItemDisabledColor = Flatten(TextFillColorDisabled, MenuBGColor);
        MainMenuBorderColor = Flatten(SurfaceStrokeColorFlyout, MenuBGColor);

        TextFGColor = Opaque(TextFillColorPrimary);
        PropPageCaptionFGColor = Opaque(TextFillColorPrimary);
        TextFGColorFade = Flatten(TextFillColorSecondary, base);
        ContentTextDisabledFGColorFade = Flatten(TextFillColorDisabled, ContentBGColor);
        ContentTextDisabledFGColorFade2 = Flatten(ControlStrongStrokeColorDisabled, ContentBGColor); //even more faded: no fainter text token, the disabled strong stroke is the nearest role

        SubmenuColor = Flatten(TextFillColorSecondary, MenuBGColor);

        WindowBorderColorLight = Flatten(SurfaceStrokeColorDefault, base);
        WindowBorderColorDim = Flatten(CardStrokeColorDefault, base);
        NoBorderColor = RGB(5, 11, 12); //seekbar frame
        GripperPatternColor = Flatten(ControlStrongStrokeColorDisabled, WindowBGColor); //gripper dots: a de-emphasised strong stroke

        ScrollBGColor = RGB(10, 20, 22); //unplayed part of the seekbar, also the scroll bar channel
        ScrollProgressColor = RGB(53, 122, 122); //played part, at least 3:1 against the unplayed part
        ScrollThumbColor = Flatten(ControlStrongFillColorDisabled, ScrollBGColor); //resting thumb: the dimmer of the two strong fills
        ScrollThumbHoverColor = Flatten(ControlStrongFillColorDefault, ScrollBGColor);
        ScrollThumbDragColor = Flatten(TextFillColorSecondary, ScrollBGColor); //dragged thumb: one step brighter than the hover fill
        ScrollButtonArrowColor = Flatten(TextFillColorSecondary, ScrollBGColor);
        ScrollButtonArrowClickColor = Flatten(TextFillColorTertiary, ScrollBGColor);
        ScrollButtonHoverColor = Flatten(SubtleFillColorSecondary, ScrollBGColor);
        ScrollButtonClickColor = Flatten(SubtleFillColorTertiary, ScrollBGColor);

        InlineEditBorderColor = Opaque(FocusStrokeColorOuter);
        TooltipBorderColor = Flatten(SurfaceStrokeColorDefault, MenuBGColor); //tooltips are filled with MenuBGColor

        GroupBoxBorderColor = Flatten(SurfaceStrokeColorDefault, WindowBGColor); //a notch stronger than fluent's own stroke, which nearly vanishes without mica behind it

        PlayerButtonHotColor = Flatten(SubtleFillColorSecondary, PlayerBGColor);
        PlayerButtonCheckedColor = Opaque(SolidBackgroundFillColorBaseAlt); //checked toolbar button: sunken onto the alternate base surface
        PlayerButtonClickedColor = Flatten(SubtleFillColorTertiary, PlayerBGColor);
        PlayerButtonBorderColor = Flatten(ControlStrokeColorDefault, PlayerBGColor);

        ButtonBorderOuterColor = Flatten(ControlStrokeColorVisible, WindowBGColor);
        ButtonBorderInnerColor = Flatten(ControlStrokeColorDefault, WindowBGColor);
        ButtonBorderSelectedKBFocusColor = Opaque(FocusStrokeColorOuter);
        ButtonBorderHoverKBFocusColor = Opaque(FocusStrokeColorOuter);
        ButtonBorderKBFocusColor = Opaque(FocusStrokeColorOuter);
        ButtonFillColor = Flatten(ControlFillColorDefault, WindowBGColor);
        ButtonFillHoverColor = Flatten(ControlFillColorSecondary, WindowBGColor);
        ButtonFillSelectedColor = Flatten(ControlFillColorTertiary, WindowBGColor);
        ButtonDisabledFGColor = Flatten(TextFillColorDisabled, WindowBGColor);

        CheckboxBorderColor = Flatten(ControlStrongStrokeColorDefault, WindowBGColor);
        CheckboxBGColor = Flatten(ControlAltFillColorSecondary, WindowBGColor);
        CheckboxBGHoverColor = Flatten(ControlAltFillColorTertiary, WindowBGColor);
        CheckboxDisabledBorderColor = Flatten(ControlStrongStrokeColorDisabled, WindowBGColor);
        CheckboxDisabledCheckedColor = Flatten(AccentFillColorDisabled, WindowBGColor); //fluent greys out the accent fill
        CheckboxDisabledGlyphColor = Flatten(TextOnAccentFillColorDisabled, CheckboxDisabledCheckedColor);

        ImageDisabledColor = Flatten(TextFillColorDisabled, WindowBGColor);

        SliderChannelColor = Flatten(ControlStrongFillColorDefault, WindowBGColor);
        SliderThumbColor = Opaque(ControlSolidFillColorDefault); //fluent slider thumb: a solid disc with an accent dot
        SliderThumbBorderColor = Flatten(ControlStrokeColorSecondary, WindowBGColor);

        EditBorderColor = Flatten(ControlStrokeColorVisible, WindowBGColor);

        //the statistics and status bars stay a dark strip under the player bars, as in the Windows 11 dark style
        InfoBarBGColor = Opaque(SolidBackgroundFillColorBaseAlt);
        InfoBarTextColor = RGB(228, 220, 205);
        InfoBarBorderColor = InfoBarBGColor;

        TreeCtrlLineColor = Flatten(ControlStrongStrokeColorDisabled, ContentBGColor); //tree connector lines: a de-emphasised strong stroke
        TreeCtrlHoverColor = Flatten(SubtleFillColorSecondary, ContentBGColor);
        TreeCtrlFocusColor = Opaque(ControlSolidFillColorDefault); //selection, as ContentSelectedColor

        CheckColor = Opaque(TextFillColorPrimary);

        ColumnHeaderHotColor = Flatten(SubtleFillColorSecondary, ContentBGColor);

        StaticEtchedColor = Flatten(DividerStrokeColorDefault, WindowBGColor);

        ListCtrlDisabledBGColor = Flatten(ControlFillColorDisabled, ContentBGColor);
        ListCtrlGridColor = Flatten(DividerStrokeColorDefault, ContentBGColor);
        ListCtrlErrorColor = Opaque(SystemFillColorCriticalBackground); //drawn behind text, so the critical background rather than the critical fill
        HeaderCtrlGridColor = Flatten(DividerStrokeColorDefault, ContentBGColor);
        AudioSwitcherGridColor = Flatten(DividerStrokeColorDefault, ContentBGColor);

        TabCtrlBorderColor = Flatten(DividerStrokeColorDefault, WindowBGColor);
        TabCtrlInactiveColor = Flatten(ControlFillColorDefault, WindowBGColor);

        StatusBarBGColor = Opaque(SolidBackgroundFillColorSecondary);
        StatusBarSeparatorColor = Flatten(DividerStrokeColorDefault, StatusBarBGColor);

        ProgressBarBGColor = Flatten(ControlStrongStrokeColorDefault, WindowBGColor); //progress track

        SubresyncFadeText1 = Flatten(TextFillColorTertiary, ContentBGColor);
        SubresyncFadeText2 = Flatten(TextFillColorDisabled, ContentBGColor);
        SubresyncActiveFadeText = Flatten(TextFillColorSecondary, ContentBGColor);
        SubresyncHLColor1 = Flatten(ControlStrongFillColorDisabled, ContentBGColor); //modified rows: a dimmed strong fill
        SubresyncHLColor2 = Flatten(ControlFillColorSecondary, ContentBGColor); //adjusted rows: a control fill
        SubresyncGridSepColor = Flatten(ControlStrongStrokeColorDefault, ContentBGColor);

        SeekbarCurrentPositionColor = RGB(220, 205, 181); //position mark, at least 3:1 against the played part
        SeekbarChapterColor = RGB(138, 129, 112); //chapter marks
        SeekbarABColor = RGB(242, 13, 13);

        //mode independent in the Windows 10 palette
        CloseHoverColor = Opaque(KelpieLight::SystemFillColorCritical); //windows 11 uses the same close red in both modes
        ClosePushColor = Flatten(ControlStrokeColorOnAccentSecondary, CloseHoverColor); //pressed close: the on-accent stroke darkens the red
        W10DarkThemeFileDialogInjectedTextColor = Opaque(TextFillColorPrimary);
        W10DarkThemeFileDialogInjectedBGColor = ControlAreaBGColor;
        W10DarkThemeFileDialogInjectedEditBorderColor = Flatten(ControlStrokeColorSecondary, ControlAreaBGColor);
        W10DarkThemeTitlebarBGColor = base;
        W10DarkThemeTitlebarInactiveBGColor = base; //windows 11 keeps the titlebar colour and dims the text when inactive
        W10DarkThemeTitlebarFGColor = Opaque(TextFillColorPrimary);
        W10DarkThemeTitlebarInactiveFGColor = Flatten(TextFillColorDisabled, base);
        W10DarkThemeTitlebarIconPenColor = Opaque(TextFillColorPrimary);
        W10DarkThemeTitlebarControlHoverBGColor = Flatten(SubtleFillColorSecondary, base);
        W10DarkThemeTitlebarInactiveControlHoverBGColor = Flatten(SubtleFillColorSecondary, base);
        W10DarkThemeTitlebarControlPushedBGColor = Flatten(SubtleFillColorTertiary, base);
        W10DarkThemeWindowBorderColor = Flatten(SurfaceStrokeColorDefault, base);
    } else {
        using namespace KelpieLight;
        drawThemedControls = true;

        const COLORREF base = Opaque(SolidBackgroundFillColorBase);
        const COLORREF layer = Flatten(LayerFillColorDefault, base); //content surfaces sit one layer above the base

        MenuBGColor = Opaque(SolidBackgroundFillColorTertiary);
        MenubarBGColor = base;
        WindowBGColor = base;
        ControlAreaBGColor = Opaque(SolidBackgroundFillColorSecondary);

        ContentBGColor = layer;
        PlaylistSelectedColor = Flatten(SubtleFillColorSecondary, ContentBGColor); //list selection is neutral, the accent goes on an indicator
        ContentSelectedColor = Opaque(SolidBackgroundFillColorBaseAlt); //neutral selection as in explorer; the light solid fill is white, so the alternate base is the nearest solid neutral
        PlayerBGColor = layer; //player bars share the content layer so the seekbar stays flush with the toolbar

        MenuSelectedColor = Flatten(SubtleFillColorSecondary, MenuBGColor); //darker than the menu, unlike the Windows 10 palette
        MenubarSelectedBGColor = Flatten(SubtleFillColorSecondary, MenubarBGColor);
        MenuSeparatorColor = Flatten(DividerStrokeColorDefault, MenuBGColor);
        MenuItemDisabledColor = Flatten(TextFillColorDisabled, MenuBGColor);
        MainMenuBorderColor = Flatten(SurfaceStrokeColorFlyout, MenuBGColor);

        TextFGColor = Flatten(TextFillColorPrimary, base);
        PropPageCaptionFGColor = TextFGColor; //the caption gradient starts at the neutral selection colour
        TextFGColorFade = Flatten(TextFillColorSecondary, base);
        ContentTextDisabledFGColorFade = Flatten(TextFillColorDisabled, ContentBGColor);
        ContentTextDisabledFGColorFade2 = Flatten(ControlStrongStrokeColorDisabled, ContentBGColor); //even more faded: no fainter text token, the disabled strong stroke is the nearest role

        SubmenuColor = Flatten(TextFillColorSecondary, MenuBGColor);

        WindowBorderColorLight = Flatten(SurfaceStrokeColorDefault, base);
        WindowBorderColorDim = Flatten(CardStrokeColorDefault, base);
        NoBorderColor = RGB(169, 188, 189); //seekbar frame
        GripperPatternColor = Flatten(ControlStrongStrokeColorDisabled, WindowBGColor); //gripper dots: a de-emphasised strong stroke

        ScrollBGColor = RGB(216, 227, 227); //unplayed part of the seekbar, also the scroll bar channel
        ScrollProgressColor = RGB(67, 133, 133); //played part, at least 3:1 against the unplayed part
        ScrollThumbColor = Flatten(ControlStrongFillColorDisabled, ScrollBGColor); //resting thumb: the dimmer of the two strong fills
        ScrollThumbHoverColor = Flatten(ControlStrongFillColorDefault, ScrollBGColor);
        ScrollThumbDragColor = Flatten(TextFillColorSecondary, ScrollBGColor); //dragged thumb: one step darker than the hover fill
        ScrollButtonArrowColor = Flatten(TextFillColorSecondary, ScrollBGColor);
        ScrollButtonArrowClickColor = Flatten(TextFillColorTertiary, ScrollBGColor);
        ScrollButtonHoverColor = Flatten(SubtleFillColorSecondary, ScrollBGColor);
        ScrollButtonClickColor = Flatten(SubtleFillColorTertiary, ScrollBGColor);

        InlineEditBorderColor = Flatten(FocusStrokeColorOuter, ContentBGColor);
        TooltipBorderColor = Flatten(SurfaceStrokeColorDefault, MenuBGColor); //tooltips are filled with MenuBGColor

        GroupBoxBorderColor = Flatten(DividerStrokeColorDefault, WindowBGColor);

        PlayerButtonHotColor = Flatten(SubtleFillColorSecondary, PlayerBGColor);
        PlayerButtonCheckedColor = Opaque(SolidBackgroundFillColorBaseAlt); //checked toolbar button: sunken onto the alternate base surface
        PlayerButtonClickedColor = Flatten(SubtleFillColorTertiary, PlayerBGColor);
        PlayerButtonBorderColor = Flatten(ControlStrokeColorDefault, PlayerBGColor);

        ButtonBorderOuterColor = Flatten(ControlStrokeColorSecondary, WindowBGColor);
        ButtonBorderInnerColor = Flatten(ControlStrokeColorDefault, WindowBGColor);
        ButtonBorderSelectedKBFocusColor = Flatten(FocusStrokeColorOuter, WindowBGColor);
        ButtonBorderHoverKBFocusColor = Flatten(FocusStrokeColorOuter, WindowBGColor);
        ButtonBorderKBFocusColor = Flatten(FocusStrokeColorOuter, WindowBGColor);
        ButtonFillColor = Flatten(ControlFillColorDefault, WindowBGColor);
        ButtonFillHoverColor = Flatten(ControlFillColorSecondary, WindowBGColor);
        ButtonFillSelectedColor = Flatten(ControlFillColorTertiary, WindowBGColor);
        ButtonDisabledFGColor = Flatten(TextFillColorDisabled, WindowBGColor);

        CheckboxBorderColor = Flatten(ControlStrongStrokeColorDefault, WindowBGColor);
        CheckboxBGColor = Flatten(ControlAltFillColorSecondary, WindowBGColor);
        CheckboxBGHoverColor = Flatten(ControlAltFillColorTertiary, WindowBGColor);
        CheckboxDisabledBorderColor = Flatten(ControlStrongStrokeColorDisabled, WindowBGColor);
        CheckboxDisabledCheckedColor = Flatten(AccentFillColorDisabled, WindowBGColor); //fluent greys out the accent fill
        CheckboxDisabledGlyphColor = Flatten(TextOnAccentFillColorDisabled, CheckboxDisabledCheckedColor);

        ImageDisabledColor = Flatten(TextFillColorDisabled, WindowBGColor);

        SliderChannelColor = Flatten(ControlStrongFillColorDefault, WindowBGColor);
        SliderThumbColor = Opaque(ControlSolidFillColorDefault); //fluent slider thumb: a solid disc with an accent dot
        SliderThumbBorderColor = Flatten(ControlStrokeColorSecondary, WindowBGColor);

        EditBorderColor = Flatten(ControlStrokeColorSecondary, WindowBGColor); //text boxes have the stronger bottom stroke

        //a black strip under light toolbars looks out of place in windows 11, so the statistics and status bars join the player bars
        InfoBarBGColor = PlayerBGColor;
        InfoBarTextColor = TextFGColor;
        InfoBarBorderColor = Flatten(DividerStrokeColorDefault, PlayerBGColor);

        TreeCtrlLineColor = Flatten(ControlStrongStrokeColorDisabled, ContentBGColor); //tree connector lines: a de-emphasised strong stroke
        TreeCtrlHoverColor = Flatten(SubtleFillColorSecondary, ContentBGColor);
        TreeCtrlFocusColor = Opaque(SolidBackgroundFillColorBaseAlt); //selection, as ContentSelectedColor

        CheckColor = TextFGColor; //only read by the Windows 10 check box images; the Windows 11 style draws its own with CheckboxGlyphColor

        ColumnHeaderHotColor = Flatten(SubtleFillColorSecondary, ContentBGColor);

        StaticEtchedColor = Flatten(DividerStrokeColorDefault, WindowBGColor);

        ListCtrlDisabledBGColor = Opaque(SolidBackgroundFillColorSecondary); //the light disabled fill is near white and vanishes over the layer, so a disabled list drops to the secondary surface
        ListCtrlGridColor = Flatten(DividerStrokeColorDefault, ContentBGColor);
        ListCtrlErrorColor = Opaque(SystemFillColorCriticalBackground); //drawn behind text, so the critical background rather than the critical fill
        HeaderCtrlGridColor = Flatten(DividerStrokeColorDefault, ContentBGColor);
        AudioSwitcherGridColor = Flatten(DividerStrokeColorDefault, ContentBGColor);

        TabCtrlBorderColor = Flatten(DividerStrokeColorDefault, WindowBGColor);
        TabCtrlInactiveColor = Flatten(ControlFillColorDefault, WindowBGColor);

        StatusBarBGColor = Opaque(SolidBackgroundFillColorSecondary);
        StatusBarSeparatorColor = Flatten(DividerStrokeColorDefault, StatusBarBGColor);

        ProgressBarBGColor = Flatten(ControlStrongStrokeColorDefault, WindowBGColor); //progress track

        SubresyncFadeText1 = Flatten(TextFillColorTertiary, ContentBGColor);
        SubresyncFadeText2 = Flatten(TextFillColorDisabled, ContentBGColor);
        SubresyncActiveFadeText = Flatten(TextFillColorSecondary, ContentBGColor);
        SubresyncHLColor1 = Flatten(ControlStrongFillColorDisabled, ContentBGColor); //modified rows: a dimmed strong fill
        SubresyncHLColor2 = Flatten(ControlAltFillColorQuarternary, ContentBGColor); //adjusted rows: the light control fills are near white and vanish over the layer, so the strongest alt fill stands in
        SubresyncGridSepColor = Flatten(ControlStrongStrokeColorDefault, ContentBGColor);

        SeekbarCurrentPositionColor = RGB(15, 31, 33); //position mark, at least 3:1 against the played part
        SeekbarChapterColor = RGB(111, 129, 131); //chapter marks
        SeekbarABColor = RGB(242, 13, 13);

        //mode independent in the Windows 10 palette, which draws these controls in dark mode only
        ComboboxArrowColor = Flatten(TextFillColorSecondary, ButtonFillColor);
        ComboboxArrowColorDisabled = Flatten(TextFillColorDisabled, ButtonFillColor);
        HeaderCtrlSortArrowColor = Flatten(TextFillColorSecondary, ContentBGColor);

        //mode independent in the Windows 10 palette; the titlebar and file dialog slots are only drawn in dark mode
        CloseHoverColor = Opaque(SystemFillColorCritical); //windows 11 uses the same close red in both modes
        ClosePushColor = Flatten(ControlStrokeColorOnAccentSecondary, CloseHoverColor); //pressed close: the on-accent stroke darkens the red
        W10DarkThemeFileDialogInjectedTextColor = TextFGColor;
        W10DarkThemeFileDialogInjectedBGColor = ControlAreaBGColor;
        W10DarkThemeFileDialogInjectedEditBorderColor = Flatten(ControlStrokeColorSecondary, ControlAreaBGColor);
        W10DarkThemeTitlebarBGColor = base;
        W10DarkThemeTitlebarInactiveBGColor = base;
        W10DarkThemeTitlebarFGColor = TextFGColor;
        W10DarkThemeTitlebarInactiveFGColor = Flatten(TextFillColorDisabled, base);
        W10DarkThemeTitlebarIconPenColor = TextFGColor;
        W10DarkThemeTitlebarControlHoverBGColor = Flatten(SubtleFillColorSecondary, base);
        W10DarkThemeTitlebarInactiveControlHoverBGColor = Flatten(SubtleFillColorSecondary, base);
        W10DarkThemeTitlebarControlPushedBGColor = Flatten(SubtleFillColorTertiary, base);
        W10DarkThemeWindowBorderColor = Flatten(SurfaceStrokeColorDefault, base);
    }
    ApplyAccentColors();
}


