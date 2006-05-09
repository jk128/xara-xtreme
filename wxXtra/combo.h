// $Id: odcombo.cpp 751 2006-03-31 15:43:49Z alex $
/* @@tag:xara-cn-tp@@ THIRD PARTY COPYRIGHT */
// The following line makes normalize.pl skip type fixing
/* SKIPFIXTYPES: START */

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/combo.h
// Purpose:     wxComboControl declaration
// Author:      Jaakko Salli
// Modified by: 
// Created:     Apr-30-2006
// RCS-ID:      
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXXTRA_COMBOCONTROL_H_BASE_
#define _WXXTRA_COMBOCONTROL_H_BASE_

#include <wx/wx.h>

/*
   A few words about all the classes defined in this file are probably in
   order: why do we need extra wxComboControl and wxComboPopup classes?

   This is because a traditional combobox is a combination of a text control
   (with a button allowing to open the pop down list) with a listbox and
   wxComboBox class is exactly such control, however we want to also have other
   combinations - in fact, we want to allow anything at all to be used as pop
   down list, not just a wxListBox.

   So we define a base wxComboControl which can use any control as pop down
   list and wxComboBox deriving from it which implements the standard wxWidgets
   combobox API. wxComboControl needs to be told somehow which control to use
   and this is done by SetPopupControl(). However, we need something more than
   just a wxControl in this method as, for example, we need to call
   SetSelection("initial text value") and wxControl doesn't have such method.
   So we also need a wxComboPopup which is just a very simple interface which
   must be implemented by a control to be usable as a popup.

   We couldn't derive wxComboPopup from wxControl as this would make it
   impossible to have a class deriving from both wxListBx and from it, so
   instead it is just a mix-in.
 */

#if wxUSE_COMBOCONTROL
#undef wxXTRA_COMBOCONTROL
#else
#define wxXTRA_COMBOCONTROL 1


class WXDLLEXPORT wxComboPopup;

//
// New window styles for wxComboControlBase
//
enum
{
    // Double-clicking a read-only combo triggers call to popup's OnComboPopup.
    // In wxOwnerDrawnComboBox, for instance, it cycles item.
    wxCC_SPECIAL_DCLICK             = 0x0100,

    // Use keyboard behaviour alternate to platform default:
    // Up an down keys will show popup instead of cycling value.
    wxCC_ALT_KEYS                   = 0x0200,

    // Dropbutton acts like standard push button.
    wxCC_STD_BUTTON                 = 0x0400,
};


// wxComboControl internal flags
enum
{
    // First those that can be passed to Customize.
    // It is Windows style for all flags to be clear.

    // Button is preferred outside the border (GTK style)
    wxCC_BUTTON_OUTSIDE_BORDER      = 0x0001,
    // Show popup on mouse up instead of mouse down (which is the Windows style)
    wxCC_POPUP_ON_MOUSE_UP          = 0x0002,
    // All text is not automatically selected on click
    wxCC_NO_TEXT_AUTO_SELECT        = 0x0004,

    // Internal use: signals creation is complete
    wxCC_IFLAG_CREATED              = 0x0100,
    // Internal use: really put button outside
    wxCC_IFLAG_BUTTON_OUTSIDE       = 0x0200,
    // Internal use: SetTextIndent has been called
    wxCC_IFLAG_INDENT_SET           = 0x0400,
    // Internal use: Set wxTAB_TRAVERSAL to parent when popup is dismissed
    wxCC_IFLAG_PARENT_TAB_TRAVERSAL = 0x0800,
};


// Flags used by PreprocessMouseEvent and HandleButtonMouseEvent
enum
{
    wxCC_MF_ON_BUTTON               =   0x0001, // cursor is on dropbutton area
};


// Namespace for wxComboControl feature flags
struct wxComboControlFeatures
{
    enum
    {
        MovableButton       = 0x0001, // Button can be on either side of control
        BitmapButton        = 0x0002, // Button may be replaced with bitmap
        ButtonSpacing       = 0x0004, // Button can have spacing from the edge
                                      // of the control
        TextIndent          = 0x0008, // SetTextIndent can be used
        PaintControl        = 0x0010, // Combo control itself can be custom painted
        PaintWritable       = 0x0020, // A variable-width area in front of writable
                                      // combo control's textctrl can be custom
                                      // painted
        Borderless          = 0x0040, // wxNO_BORDER window style works

        // There are no feature flags for...
        // PushButtonBitmapBackground - if its in wxRendererNative, then it should be
        //   not an issue to have it automatically under the bitmap.

        All                 = MovableButton|BitmapButton|
                              ButtonSpacing|TextIndent|
                              PaintControl|PaintWritable|
                              Borderless,
    };
};


class WXDLLEXPORT wxComboControlBase : public wxControl
{
    friend class wxComboPopup;
public:
    // ctors and such
    wxComboControlBase() : wxControl() { Init(); }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxValidator& validator,
                const wxString& name);

    virtual ~wxComboControlBase();

    // show/hide popup window
    virtual void ShowPopup();
    virtual void HidePopup();

    // Override for totally custom combo action
    virtual void OnButtonClick();

    // return true if the popup is currently shown
    bool IsPopupShown() const { return m_isPopupShown; }

    // set interface class instance derived from wxComboPopup
    void SetPopupControl( wxComboPopup* iface );

    // get interface class instance derived from wxComboPopup
    wxComboPopup* GetPopup() const { return m_popupInterface; }

    // get the popup window containing the popup control
    wxWindow *GetPopupWindow() const { return m_winPopup; }

    // get the popup control/panel in window
    wxWindow *GetPopupControl() const { return m_popup; }

    // Get the text control which is part of the combobox.
    wxTextCtrl *GetTextCtrl() const { return m_text; }

    // get the dropdown button which is part of the combobox
    // note: its not necessarily a wxButton or wxBitmapButton
    wxWindow *GetButton() const { return m_btn; }

    // forward these methods to all subcontrols
    virtual bool Enable(bool enable = true);
    virtual bool Show(bool show = true);
    virtual bool SetFont(const wxFont& font);
#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );
#endif

    // wxTextCtrl methods - for readonly combo they should return
    // without errors.
    virtual wxString GetValue() const;
    virtual void SetValue(const wxString& value);
    virtual void Copy();
    virtual void Cut();
    virtual void Paste();
    virtual void SetInsertionPoint(long pos);
    virtual void SetInsertionPointEnd();
    virtual long GetInsertionPoint() const;
    virtual long GetLastPosition() const;
    virtual void Replace(long from, long to, const wxString& value);
    virtual void Remove(long from, long to);
    virtual void SetSelection(long from, long to);
    virtual void Undo();

    //
    // Popup customization methods
    //

    // Sets minimum width of the popup. If wider than combo control, it will extend to the left.
    // Remarks:
    // * Value -1 indicates the default.
    // * Custom popup may choose to ignore this (wxOwnerDrawnComboBox does not).
    void SetPopupMinWidth( int width )
    {
        m_widthMinPopup = width;
    }

    // Sets preferred maximum height of the popup.
    // Remarks:
    // * Value -1 indicates the default.
    // * Custom popup may choose to ignore this (wxOwnerDrawnComboBox does not).
    void SetPopupMaxHeight( int height )
    {
        m_heightPopup = height;
    }

    // Extends popup size horizontally, relative to the edges of the combo control.
    // Remarks:
    // * Popup minimum width may override extLeft (ie. it has higher precedence).
    // * Values 0 indicate default.
    // * Custom popup may not take this fully into account (wxOwnerDrawnComboBox takes).
    void SetPopupExtents( int extLeft, int extRight )
    {
        m_extLeft = extLeft;
        m_extRight = extRight;
    }

    // Set width, in pixels, of custom paint area in writable combo.
    // In read-only, used to indicate area that is not covered by the
    // focus rectangle (which may or may not be drawn, depending on the
    // popup type).
    void SetCustomPaintWidth( int width );
    int GetCustomPaintWidth() const { return m_widthCustomPaint; }

    // Set side of the control to which the popup will align itself.
    // Valid values are wxLEFT, wxRIGHT and 0. The default value 0 wmeans
    // that the side of the button will be used.
    void SetPopupAnchor( int anchorSide )
    {
        m_anchorSide = anchorSide;
    }

    // Set position of dropdown button.
    //   width: 0 > for specific custom width, negative to adjust to smaller than default
    //   height: 0 > for specific custom height, negative to adjust to smaller than default
    //   side: wxLEFT or wxRIGHT, indicates on which side the button will be placed.
    //   spacingX: empty space on sides of the button. Default is 0.
    // Remarks:
    //   There is no spacingY - the button will be centered vertically.
    void SetButtonPosition( int width = 0, int height = 0, int side = wxRIGHT,
                            int spacingX = 0 /*, int spacingY = 0*/ );


    //
    // Sets dropbutton to be drawn with custom bitmaps.
    //
    //  bmpNormal: drawn when cursor is not on button
    //  blankButtonBg: Draw blank button background below the image.
    //                 NOTE! This is only properly supported on platforms with appropriate
    //                       method in wxRendererNative.
    //  bmpPressed: drawn when button is depressed
    //  bmpHover: drawn when cursor hovers on button. This is ignored on platforms
    //            that do not generally display hover differently.
    //  bmpDisabled: drawn when combobox is disabled.
    void SetButtonBitmaps( const wxBitmap& bmpNormal,
                           bool blankButtonBg = false,
                           const wxBitmap& bmpPressed = wxNullBitmap,
                           const wxBitmap& bmpHover = wxNullBitmap,
                           const wxBitmap& bmpDisabled = wxNullBitmap );

    //
    // This will set the space in pixels between left edge of the control and the
    // text, regardless whether control is read-only (ie. no wxTextCtrl) or not.
    // Platform-specific default can be set with value-1.
    // Remarks
    // * This method may do nothing on some native implementations.
    void SetTextIndent( int indent );

    // Returns actual indentation in pixels.
    wxCoord GetTextIndent() const
    {
        return m_absIndent;
    }

    //
    // Utilies needed by the popups or native implementations
    //

    // Draws focus background (on combo control) in a way typical on platform.
    // Unless you plan to paint your own focus indicator, you should always call this
    // in your wxComboPopup::PaintComboControl implementation.
    // In addition, it sets pen and text colour to what looks good and proper
    // against the background.
    // flags: wxRendererNative flags: wxCONTROL_ISSUBMENU: is drawing a list item instead of combo control
    //                                wxCONTROL_SELECTED: list item is selected
    //                                wxCONTROL_DISABLED: control/item is disabled
    virtual void DrawFocusBackground( wxDC& dc, const wxRect& rect, int flags );

    // Returns true if focus indicator should be drawn.
    bool ShouldDrawFocus() const
    {
        const wxWindow* curFocus = FindFocus();
        return ( !m_isPopupShown &&
                 (curFocus == this || (m_btn && curFocus == m_btn)) &&
                 (m_windowStyle & wxCB_READONLY) );
    }

    // These methods return references to appropriate dropbutton bitmaps
    const wxBitmap& GetBitmapNormal() const { return m_bmpNormal; }
    const wxBitmap& GetBitmapPressed() const { return m_bmpPressed; }
    const wxBitmap& GetBitmapHover() const { return m_bmpHover; }
    const wxBitmap& GetBitmapDisabled() const { return m_bmpDisabled; }

    // Return internal flags
    wxUint32 GetInternalFlags() const { return m_iFlags; }

    // Return true if Create has finished
    bool IsCreated() const { return m_iFlags & wxCC_IFLAG_CREATED ? true : false; }

    // Popup may use these as callbacks to measure and draw list items.
    // (wxOwnerDrawnComboBox uses these, obviously)
    // item: -1 means item is the combo control itself
    // flags: wxCC_PAINTING_CONTROL is set if painting to combo control instead of list
    // return value: OnDrawListItem must return true if it did anything
    virtual bool OnDrawListItem( wxDC& dc, const wxRect& rect, int item, int flags );

    // Return item height, or -1 for text height (default)
    virtual wxCoord OnMeasureListItem( int item );

    // Return item width, or -1 for calculating from text extent (default)
    virtual wxCoord OnMeasureListItemWidth( int item );

    // NOTE:
    // I basicly needed to add callback methods into wxComboControlBase - otherwise it
    // will not be easily possible to use wxVListBoxComboPopup from simultaneously existing
    // wxComboControl and wxGenericComboControl (since some native implementations
    // might not have all the features, I really would like to have this options).

    // common code to be called on popup hide/dismiss
    void OnPopupDismiss();

protected:

    //
    // Override these for customization purposes
    //

    // called from wxSizeEvent handler
    virtual void OnResize() = 0;

    // Return native text identation (for pure text, not textctrl)
    virtual wxCoord GetNativeTextIndent() const;

    // Called in syscolourchanged handler and base create
    virtual void OnThemeChange();

    // Creates wxTextCtrl.
    //   extraStyle: Extra style parameters
    void CreateTextCtrl( int extraStyle, const wxValidator& validator );

    // Installs standard input handler to combo (and optionally to the textctrl)
    void InstallInputHandlers( bool alsoTextCtrl = true );

    // Draws dropbutton. Using wxRenderer or bitmaps, as appropriate.
    void DrawButton( wxDC& dc, const wxRect& rect, bool paintBg = true );

    // Call if cursor is on button area or mouse is captured for the button.
    //bool HandleButtonMouseEvent( wxMouseEvent& event, bool isInside );
    bool HandleButtonMouseEvent( wxMouseEvent& event, int flags );

    // Conversion to double-clicks and some basic filtering
    // returns true if event was consumed or filtered (event type is also set to 0 in this case)
    //bool PreprocessMouseEvent( wxMouseEvent& event, bool isOnButtonArea );
    bool PreprocessMouseEvent( wxMouseEvent& event, int flags );

    //
    // This will handle left_down and left_dclick events outside button in a Windows-like manner.
    // If you need alternate behaviour, it is recommended you manipulate and filter events to it
    // instead of building your own handling routine (for reference, on wxEVT_LEFT_DOWN it will
    // toggle popup and on wxEVT_LEFT_DCLICK it will do the same or run the popup's dclick method,
    // if defined - you should pass events of other types of it for common processing).
    void HandleNormalMouseEvent( wxMouseEvent& event );

    // Creates popup window, calls interface->Create(), etc
    void CreatePopup();

    // override the base class virtuals involved in geometry calculations
    virtual void DoMoveWindow(int x, int y, int width, int height);
    virtual wxSize DoGetBestSize() const;

    // Recalculates button and textctrl areas. Called when size or button setup change.
    //   btnWidth: default/calculated width of the dropbutton. 0 means unchanged,
    //             just recalculate.
    void CalculateAreas( int btnWidth = 0 );

    // Standard textctrl positioning routine. Just give it platform-dependant
    // textctrl coordinate adjustment.
    void PositionTextCtrl( int textCtrlXAdjust, int textCtrlYAdjust );

    // event handlers
    void OnSizeEvent( wxSizeEvent& event );
    void OnFocusEvent(wxFocusEvent& event);
    void OnTextCtrlEvent(wxCommandEvent& event);
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    // Set customization flags (directs how wxComboControlBase helpers behave)
    void Customize( wxUint32 flags ) { m_iFlags |= flags; }

    // Dispatches size event and refreshes
    void RecalcAndRefresh();

    // Used by OnPaints of derived classes
    wxBitmap& GetBufferBitmap(const wxSize& sz) const;

    // This is used when m_text is hidden (readonly).
    wxString                m_valueString;

    // the text control and button we show all the time
    wxTextCtrl*             m_text;
    wxWindow*               m_btn;

    // wxPopupWindow or similar containing the window managed by the interface.
    wxWindow*               m_winPopup;

    // the popup control/panel
    wxWindow*               m_popup;

    // popup interface
    wxComboPopup*         m_popupInterface;

    // this is for this control itself
    wxEvtHandler*           m_extraEvtHandler;

    // this is for text
    wxEvtHandler*           m_textEvtHandler;

    // this is for the top level window
    wxEvtHandler*           m_toplevEvtHandler;

    // this is for the control in popup
    wxEvtHandler*           m_popupExtraHandler;

    // needed for "instant" double-click handling
    wxLongLong              m_timeLastMouseUp;

    // used to prevent immediate re-popupping incase closed popup
    // by clicking on the combo control (needed because of inconsistent
    // transient implementation across platforms).
    wxLongLong              m_timeCanAcceptClick;

    // how much popup should expand to the left/right of the control
    wxCoord                 m_extLeft;
    wxCoord                 m_extRight;

    // minimum popup width
    wxCoord                 m_widthMinPopup;

    // preferred popup height
    wxCoord                 m_heightPopup;

    // how much of writable combo is custom-paint by callback?
    // also used to indicate area that is not covered by "blue"
    // selection indicator.
    wxCoord                 m_widthCustomPaint;

    // absolute text indentation, in pixels
    wxCoord                 m_absIndent;

    // side on which the popup is aligned
    int                     m_anchorSide;

    // Width of the "fake" border
    wxCoord                 m_widthCustomBorder;

    // The button and textctrl click/paint areas
    wxRect                  m_tcArea;
    wxRect                  m_btnArea;

    // current button state (uses renderer flags)
    int                     m_btnState;

    // button position
    int                     m_btnWid;
    int                     m_btnHei;
    int                     m_btnSide;
    int                     m_btnSpacingX;

    // last default button width
    int                     m_btnWidDefault;

    // custom dropbutton bitmaps
    wxBitmap                m_bmpNormal;
    wxBitmap                m_bmpPressed;
    wxBitmap                m_bmpHover;
    wxBitmap                m_bmpDisabled;

    // area used by the button
    wxSize                  m_btnSize;

    // platform-dependant customization and other flags
    wxUint32                m_iFlags;

    // draw blank button background under bitmap?
    bool                    m_blankButtonBg;

    // is the popup window currenty shown?
    bool                    m_isPopupShown;

    // Set to 1 on mouse down, 0 on mouse up. Used to eliminate down-less mouse ups.
    bool                    m_downReceived;

private:
    void Init();

    DECLARE_EVENT_TABLE()

    DECLARE_ABSTRACT_CLASS(wxComboControlBase)
};


// ----------------------------------------------------------------------------
// wxComboPopup is the interface which must be implemented by a control to be
// used as a popup by wxComboControl
// ----------------------------------------------------------------------------


// wxComboPopup internal flags
enum
{
    wxCP_IFLAG_CREATED      = 0x0001, // Set by wxComboControlBase after Create is called
};


class WXDLLEXPORT wxComboPopup
{
    friend class wxComboControlBase;
public:
    wxComboPopup(wxComboControlBase *combo)
    {
        m_combo = combo;
        m_iFlags = 0;
    }

    virtual ~wxComboPopup();

    // Create the popup child control.
    // Return true for success.
    virtual bool Create(wxWindow* parent) = 0;

    // We must have an associated control which is subclassed by the combobox.
    virtual wxWindow *GetControl() = 0;

    // Called immediately after the popup is shown
    virtual void OnPopup();

    // Called when popup is dismissed
    virtual void OnDismiss();

    // Called just prior to displaying popup.
    // Default implementation does nothing.
    virtual void SetStringValue( const wxString& value );

    // Gets displayed string representation of the value.
    virtual wxString GetStringValue() const = 0;

    // This is called to custom paint in the combo control itself (ie. not the popup).
    // Default implementation draws value as string.
    virtual void PaintComboControl( wxDC& dc, const wxRect& rect );

    // Receives key events from the parent wxComboControl.
    // Events not handled should be skipped, as usual.
    virtual void OnComboKeyEvent( wxKeyEvent& event );

    // Implement if you need to support special action when user
    // double-clicks on the parent wxComboControl.
    virtual void OnComboDoubleClick();

    // Return final size of popup. Called on every popup, just prior to OnShow.
    // minWidth = preferred minimum width for window
    // prefHeight = preferred height. Only applies if > 0,
    // maxHeight = max height for window, as limited by screen size
    //   and should only be rounded down, if necessary.
    virtual wxSize GetAdjustedSize( int minWidth, int prefHeight, int maxHeight );

    // Return true if you want delay call to Create until the popup is shown
    // for the first time. It is more efficient, but note that it is often
    // more convenient to have the control created immediately.
    // Default returns false.
    virtual bool LazyCreate();

    //
    // Utilies
    //

    // Hides the popup
    void Dismiss();

    // Returns true if Create has been called.
    bool IsCreated() const
    {
        return (m_iFlags & wxCP_IFLAG_CREATED) ? true : false;
    }

protected:
    wxComboControlBase* m_combo;
    wxUint32            m_iFlags;
};


// ----------------------------------------------------------------------------
// include the platform-dependent header defining the real class
// ----------------------------------------------------------------------------

// Any ports may need generic as an alternative
#include "combog.h"

#endif // wxUSE_COMBOCONTROL

#endif
    // _WX_COMBOCONTROL_H_BASE_