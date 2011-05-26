import Qt 4.7
import MeeGo.Ux.Gestures 0.1

/*!
   \qmlclass GestureMouseArea
   \title GestureMouseArea
   \section1 GestureMouseArea
         this is a transition component for using Gestures as a MouseArea.
	 There is no need to change the API and the usage of your MouseArea,
	 just switch from MouseArea to GestureMouseArea QML element.
	 all the signals have instead of an MouseEvent the parameters available:
	    int mouseX
	    int mouseY
	    bool wasHeld
	 as a replacement for them.

  \section2 API Properties
  \qmlproperty bool enabled
  \qmlcm determine wheather the area is enabled or not
  \qmlproperty int mouseX.
  \qmlcm the current mouseX value
  \qmlproperty int mouseY
  \qmlcm the current mouseY value
  \qmlproperty bool pressed
  \qmlcm determine if the mouseArea was pressed
  \qmlproperty bool hoverEnabled
  \qmlcm if set to true, the GestureMouseArea will signal any hover activity
  
  \section2 Signals
  \qmlfn onCanceled
  \qmlcm emitted if.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onClicked
  \qmlcm emitted if.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onDoubleClicked
  \qmlcm emitted if a double click was recognized.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onEntered
  \qmlcm emitted if the the area is hovered.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onExited
  \qmlcm emitted if the hover is exited.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onPositionChanged
  \qmlcm emitted if the positionChanged.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onPressandHold
  \qmlcm emitted if pressed and hold.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onPressed
  \qmlcm emitted if the area was pressed.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam
  \qmlfn onReleased
  \qmlcm emitted if the area was released.
        \param int mouseX
        \qmlpcm x of the mouse event \endparam
        \param int mouseY
        \qmlpcm x of the mouse event \endparam
        \param int wasHeld
        \qmlpcm true Tap is on hold \endparam

  \section2 Functions
  \qmlnone

  \section2 Example
  \qml
    import MeeGo.Labs.Components 0.1

    GestureMouseArea {
      id: mouseArea
      anchors.fill: parent

      onClicked: {
	  aProperty = true
	  aObject.x = mouseX
	  aObject.y = mouseY
      }

      onPressAndHold: {
	  aProperty = false
	  aObject.x = mouseX
	  aObject.y = mouseY
      }
    }
  \endqml
*/

GestureArea {
  id: gestureMouseArea

  property bool enabled: true
  property bool hoverEnabled: false
  property bool isPressed: false

  property int mouseX: 0
  property int mouseY: 0

  signal clicked( int mouseX, int mouseY, bool wasHeld )
  signal pressed( int mouseX, int mouseY, bool wasHeld )
  signal doubleClicked( int mouseX, int mouseY, bool wasHeld )
  signal released( int mouseX, int mouseY, bool wasHeld )
  signal entered( int mouseX, int mouseY, bool wasHeld )
  signal exited( int mouseX, int mouseY, bool wasHeld )
  signal positionChanged( int mouseX, int mouseY, bool wasHeld )
  signal pressAndHold( int mouseX, int mouseY, bool wasHeld )

  function setPressed( x, y ) {     
      mouseX = x
      mouseY = y
      isPressed = true
      pressed( x, y, false )
  }

  function setClicked( x, y ) {     
      mouseX = x
      mouseY = y
      isPressed = true
      clicked( x, y, false  )
  }
  function setDoubleClicked( x, y ) {
      mouseX = x
      mouseY = y
      isPressed = true
      doubleClicked( x, y, false  )
  }

  function setClickAndHold( x, y ) {     
      mouseX = x
      mouseY = y
      isPressed = true      
      pressAndHold(  x, y, true  )
  }

  function setReleased( x, y ) {
      mouseX = x
      mouseY = y
      isPressed = false      
      released(  x, y, false  )
  }

  function setEnter( x, y ) {
      mouseX = x
      mouseY = y
      if( hoverEnabled ) {
	  entered(  x, y, false  )
      }
  }

  function setExit( x, y ) {
      mouseX = x
      mouseY = y
      isPressed = false
      if( hoverEnabled ) {
	  exited(  x, y, false  )
      }
  }

  function setPositionChanged( x, y ) {
      mouseX = x
      mouseY = y
      positionChanged( x, y, false )
  }

  Tap {
     when: gestureMouseArea.enabled
     onStarted: {
	setEnter( gesture.position.x, gesture.position.y )
	setPressed( gesture.position.x, gesture.position.y )
     }
     onUpdated: setClicked( gesture.position.x, gesture.position.y )
     onCanceled: { 
	setReleased( gesture.position.x, gesture.position.y )	
	setExit( gesture.position.x, gesture.position.y ) 
     }
     onFinished: { 
	setClicked( gesture.position.x, gesture.position.y ); 
	setExit( gesture.position.x, gesture.position.y ) 
     }
  }

  TapAndHold {
   when: gestureMouseArea.enabled
     onStarted: {
	setEnter( gesture.position.x, gesture.position.y )
	setPressed( gesture.position.x, gesture.position.y )
	setClickAndHold( gesture.position.x, gesture.position.y )
     }
     onUpdated: setClickAndHold( gesture.position.x, gesture.position.y )
     onCanceled: { 
	setReleased( gesture.position.x, gesture.position.y )	
	setExit( gesture.position.x, gesture.position.y ) 
     }
     onFinished: { 
	setClickAndHold( gesture.position.x, gesture.position.y ); 
	setExit( gesture.position.x, gesture.position.y ) 
     }
  }

  Pan {
    when: gestureMouseArea.enabled
    onStarted: gestureMouseArea.setEnter( gesture.position.x, gesture.position.y )
    onUpdated: gestureMouseArea.setPositionChanged( gesture.position.x, gesture.position.y )
    onCanceled: { gestureMouseArea.setExit( gesture.position.x, gesture.position.y ) }
    onFinished: { gestureMouseArea.setExit( gesture.position.x, gesture.position.y ) }
  }

  Pinch {
    when: gestureMouseArea.enabled
    onStarted: gestureMouseArea.setEnter( gesture.position.x, gesture.position.y )    
    onCanceled: { gestureMouseArea.setExit( gesture.position.x, gesture.position.y ) }
    onFinished: { gestureMouseArea.setExit( gesture.position.x, gesture.position.y ) }  
 }

  Swipe {
    when: gestureMouseArea.enabled
    onStarted: gestureMouseArea.setEnter( gesture.position.x, gesture.position.y )    
    onCanceled: { gestureMouseArea.setExit( gesture.position.x, gesture.position.y ) }
    onFinished: { gestureMouseArea.setExit( gesture.position.x, gesture.position.y ) }
  }
 
}