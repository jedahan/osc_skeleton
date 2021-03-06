#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    oscReceiver.setup(PORT);
    
    // initialize to HUB_IP for heartbeat?
    oscSender.setup(HUB_IP, PORT);
    
    // initialize interval time
    lastCallHomeTime = 0;
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    int time = ofGetElapsedTimeMillis();
    
    if (time - lastCallHomeTime > CALL_HOME_INTERVAL) {
        lastCallHomeTime = time;
        callHome();
    }
    
    // check for waiting messages
	while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        // handle heartbeat
        if(m.getAddress() == "/pulse"){
			handlePulse(&m);
		}
        
        // example of handling a /mouse/pressed message
        if(m.getAddress() == "/mouse/moved"){
			cout << m.getRemoteIp() << " mouse moved to " << m.getArgAsInt32(0) << ", " << m.getArgAsInt32(1) << endl;
            
            // REACT to the /mouse/pressed message!
		}
        
        // example of handling a /mouse/moved message
        if(m.getAddress() == "/mouse/pressed"){
			cout << m.getRemoteIp() << " mouse pressed at " << m.getArgAsInt32(0) << ", " << m.getArgAsInt32(1) << endl;
            
            // REACT to the /mouse/moved message!
		}
        
        // example of handling a /key/pressed message
        if(m.getAddress() == "/key/pressed"){
			cout << m.getRemoteIp() << " key pressed " << m.getArgAsInt32(0) << endl;
            
            // REACT to the /mouse/moved message!
		}
	}
}

void ofApp::callHome() {
    cout << "calling home" << endl;
    
    ofxOscMessage m;
	m.setAddress("/call/home");
	oscSender.sendMessage(m);
}

void ofApp::handlePulse(ofxOscMessage *m) {
    // the pulse message passes along all client IPs that have registered with the hub
    // so that we can register them in each individual client node, removing the reliance
    // on the central hub to pass messages.
    
    cout << "pulse received" << endl;
    
    int numArgs = m->getNumArgs();
    for(int i=0; i<numArgs; i++) {
        string ip = m->getArgAsString(i);
        
        // Check to see if this IP already is registered as a client
        map<string, ofxOscSender>::iterator it = clients.find(ip);
        if(it == clients.end()) {
            // no client found for this IP, add it to the map of clients.
            cout << "registering new client" << endl;
            ofxOscSender _sender;
            _sender.setup(ip, PORT);
            clients[ip] = _sender;
        }
    }
}

void ofApp::sendMessage(ofxOscMessage &m) {
    
    // here we iterate through the registered clients, sending the heartbeat message to them all.
    
    for(map<string, ofxOscSender>::iterator i = clients.begin(); i != clients.end(); i++) {
        // iterator->first = key (string ip)
        // iterator->second = value (ofxOscSender sender)
        ofxOscSender & _sender = i->second;
        cout << "sending " << m.getAddress() << " to " << i->first << endl;
        _sender.sendMessage(m);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    cout << "mouse moved locally" << endl;
    ofxOscMessage m;
    m.setAddress("/mouse/moved");
    m.addIntArg(x);
    m.addIntArg(y);
    sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    cout << "mouse pressed locally" << endl;
    ofxOscMessage m;
    m.setAddress("/mouse/pressed");
    m.addIntArg(x);
    m.addIntArg(y);
    sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
