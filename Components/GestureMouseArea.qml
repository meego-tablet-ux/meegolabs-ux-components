import Qt 4.7
import MeeGo.Ux.Gestures 0.1

// This is a transition component for using Gestures as a MouseArea
// there is no need to change the API and the usage of your MouseArea.
// Just switch from MouseArea to GestureMouseArea QML element.
// Attention: Signals will have no MouseEvent anymore, but expose 3 parameter:
// x, y and wasHeld

GestureArea {
  id: gestureMouseArea

  property bool enabled: true
  property bool hoverEnabled: false
  property bool pressed: false

  property int mouseX: 0
  property int mouseY: 0

  signal clicked( int x, int y, bool wasHeld )
  signal pressed( int x, int y, bool wasHeld )
  signal doubleClicked( int x, int y, bool wasHeld )
  signal released( int x, int y, bool wasHeld )
  signal entered( int x, int y, bool wasHeld )
  signal exited( int x, int y, bool wasHeld )
  signal positionChanged( int x, int y, bool wasHeld )
  signal pressAndHold( int x, int y, bool wasHeld )

  function setPressed( x, y ) {     
      mouseX = x
      mouseY = y
      pressed = true
      pressed( x, y, false )
  }

  function setClicked( x, y ) {     
      mouseX = x
      mouseY = y
      pressed = true
      clicked( x, y, false  )
  }
  function setDoubleClicked( x, y ) {
      mouseX = x
      mouseY = y
      pressed = true
      doubleClicked( x, y, false  )
  }

  function setClickAndHold( x, y ) {     
      mouseX = x
      mouseY = y
      pressed = true      
      pressAndHold(  x, y, true  )
  }

  function setReleased( x, y ) {
      mouseX = x
      mouseY = y
      pressed = false      
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
      pressed = false
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