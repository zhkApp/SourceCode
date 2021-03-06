/************************************************************************
	filename: 	CEGUIPushButton.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of PushButton widget base class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "elements/CEGUIPushButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	constants
*************************************************************************/
// event strings
const String PushButton::EventNamespace("PushButton");
const String PushButton::EventClicked( (utf8*)"Clicked" );
// properties
PushButtonProperties::NormalImage PushButton::d_normalImageProperty;
PushButtonProperties::PushedImage PushButton::d_pushedImageProperty;
PushButtonProperties::HoverImage  PushButton::d_hoverImageProperty;
PushButtonProperties::DisabledImage  PushButton::d_disabledImageProperty;
PushButtonProperties::UseStandardImagery PushButton::d_useStandardImageryProperty;
PushButtonProperties::TextXOffset PushButton::d_textXOffsetProperty;


/*************************************************************************
	Constructor
*************************************************************************/
PushButton::PushButton(const String& type, const String& name) :
	ButtonBase(type, name)
{
    // default options
    d_autoscaleImages		= true;
    d_useStandardImagery	= true;
    d_useNormalImage		= false;
    d_useHoverImage			= false;
    d_usePushedImage		= false;
    d_useDisabledImage		= false;
    d_textXOffset = 0.0f;

    // add properties for push button class
    addPushButtonProperties();
}


/*************************************************************************
	Destructor
*************************************************************************/
PushButton::~PushButton(void)
{
}

bool PushButton::isStandardImageryEnabled(void) const
{
    return d_useStandardImagery;
}

bool PushButton::isCustomImageryAutoSized(void) const
{
    return d_autoscaleImages;
}

const RenderableImage* PushButton::getNormalImage(void) const
{
    return d_useNormalImage ? &d_normalImage : static_cast<const RenderableImage*>(0);
}

const RenderableImage* PushButton::getHoverImage(void) const
{
    return d_useHoverImage ? &d_hoverImage : static_cast<const RenderableImage*>(0);
}

const RenderableImage* PushButton::getPushedImage(void) const
{
    return d_usePushedImage ? &d_pushedImage : static_cast<const RenderableImage*>(0);
}

const RenderableImage* PushButton::getDisabledImage(void) const
{
    return d_useDisabledImage ? &d_disabledImage : static_cast<const RenderableImage*>(0);
}

float PushButton::getTextXOffset(void) const
{
    return d_textXOffset;
}

void PushButton::setStandardImageryEnabled(bool setting)
{
    if (d_useStandardImagery != setting)
    {
        d_useStandardImagery = setting;
        requestRedraw();
    }
}

void PushButton::setCustomImageryAutoSized(bool setting)
{
    // if we are enabling auto-sizing, scale images for current size
    if (setting && setting != d_autoscaleImages)
    {
        Rect area(0, 0, getAbsoluteWidth(), getAbsoluteHeight());
        d_normalImage.setRect(area);
        d_hoverImage.setRect(area);
        d_pushedImage.setRect(area);
        d_disabledImage.setRect(area);

        requestRedraw();
    }

    d_autoscaleImages = setting;
}

void PushButton::setNormalImage(const RenderableImage* image)
{
    if (image)
    {
        d_useNormalImage = true;
        d_normalImage = *image;
        d_normalImage.setRect(Rect(0, 0, getAbsoluteWidth(), getAbsoluteHeight()));
    }
    else
    {
        d_useNormalImage = false;
    }

    requestRedraw();
}

void PushButton::setHoverImage(const RenderableImage* image)
{
    if (image)
    {
        d_useHoverImage = true;
        d_hoverImage = *image;
        d_hoverImage.setRect(Rect(0, 0, getAbsoluteWidth(), getAbsoluteHeight()));
    }
    else
    {
        d_useHoverImage = false;
    }

    requestRedraw();
}

void PushButton::setPushedImage(const RenderableImage* image)
{
    if (image)
    {
        d_usePushedImage = true;
        d_pushedImage = *image;
        d_pushedImage.setRect(Rect(0, 0, getAbsoluteWidth(), getAbsoluteHeight()));
    }
    else
    {
        d_usePushedImage = false;
    }

    requestRedraw();
}

void PushButton::setDisabledImage(const RenderableImage* image)
{
    if (image)
    {
        d_useDisabledImage = true;
        d_disabledImage = *image;
        d_disabledImage.setRect(Rect(0, 0, getAbsoluteWidth(), getAbsoluteHeight()));
    }
    else
    {
        d_useDisabledImage = false;
    }

    requestRedraw();
}

void PushButton::setTextXOffset(float offset)
{
    d_textXOffset = offset;
}

/*************************************************************************
	handler invoked internally when the button is clicked.	
*************************************************************************/
void PushButton::onClicked(WindowEventArgs& e)
{
	fireEvent(EventClicked, e, EventNamespace);
}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void PushButton::onMouseButtonUp(MouseEventArgs& e)
{
	if ((e.button == LeftButton) && isPushed())
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet != NULL)
		{
			// if mouse was released over this widget
			if (this == sheet->getChildAtPosition(e.position))
			{
				// fire event
				WindowEventArgs args(this);
				onClicked(args);
			}

		}

		e.handled = true;
	}

	// default handling
	ButtonBase::onMouseButtonUp(e);
}

void PushButton::onSized(WindowEventArgs& e)
{
    // default processing
    ButtonBase::onSized(e);

    // scale images if required.
    if (d_autoscaleImages)
    {
        Rect area(0, 0, getAbsoluteWidth(), getAbsoluteHeight());
        d_normalImage.setRect(area);
        d_hoverImage.setRect(area);
        d_pushedImage.setRect(area);
        d_disabledImage.setRect(area);

        e.handled = true;
    }

}

void PushButton::resizeWithText()
{
	Size size = getRenderSize();
	setSize( Absolute, size );

	Point newPos;
	newPos.d_x = m_ptHookPosition.d_x;
	newPos.d_y = m_ptHookPosition.d_y;
	switch( m_HookMode )
	{
	case Hook_Left:
		newPos.d_y -= d_pixelSize.d_height / 2;
		break;
	case Hook_Right:
		newPos.d_x -= d_pixelSize.d_width;
		newPos.d_y -= d_pixelSize.d_height / 2;
		break;
	case Hook_Top:
		newPos.d_x -= d_pixelSize.d_width / 2;
		break;
	case Hook_Bottom:
		newPos.d_x -= d_pixelSize.d_width / 2;
		newPos.d_y -= d_pixelSize.d_height;
		break;
	case Hook_LeftTop:
		break;
	case Hook_LeftBottom:
		newPos.d_y -= d_pixelSize.d_height;
		break;
	case Hook_RightTop:
		newPos.d_x -= d_pixelSize.d_width;
		break;
	case Hook_RightBottom:
		newPos.d_x -= d_pixelSize.d_width;
		newPos.d_y -= d_pixelSize.d_height;
		break;
	case Hook_Center:
		newPos.d_x -= d_pixelSize.d_width / 2;
		newPos.d_y -= d_pixelSize.d_height / 2;
		break;
	default:
		break;
	}
	UVector2  relativePos;
	d_area.setPosition( UVector2(cegui_absdim(PixelAligned(newPos.d_x)), cegui_absdim(PixelAligned( newPos.d_y) ) ) );
}

void PushButton::addPushButtonProperties(void)
{
	CEGUI_START_ADD_STATICPROPERTY( PushButton );
       CEGUI_ADD_STATICPROPERTY( &d_normalImageProperty );
       CEGUI_ADD_STATICPROPERTY( &d_pushedImageProperty );
       CEGUI_ADD_STATICPROPERTY( &d_hoverImageProperty );
       CEGUI_ADD_STATICPROPERTY( &d_disabledImageProperty );
       CEGUI_ADD_STATICPROPERTY( &d_useStandardImageryProperty );
       CEGUI_ADD_STATICPROPERTY( &d_textXOffsetProperty );
	CEGUI_END_ADD_STATICPROPERTY
}

} // End of  CEGUI namespace section
