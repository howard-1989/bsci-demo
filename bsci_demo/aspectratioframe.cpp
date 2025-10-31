#include "aspectratioframe.h"
#include <QSizePolicy>

AspectRatioFrame::AspectRatioFrame( QWidget * parent )
    : QFrame( parent ), m_aspectRatio( 16.0 / 9.0 )
{

    setStyleSheet( "background-color: darkblue; border: 2px solid white;" );

    QSizePolicy policy( QSizePolicy::Expanding, QSizePolicy::Preferred );

    policy.setHeightForWidth( TRUE );

    setSizePolicy( policy );

}

void AspectRatioFrame::setAspectRatio( double ratio )
{

    m_aspectRatio = ratio;

}

bool AspectRatioFrame::hasHeightForWidth() const
{

    return TRUE;

}

int AspectRatioFrame::heightForWidth( int w ) const
{

    return static_cast< int >( w / m_aspectRatio );
    
}
