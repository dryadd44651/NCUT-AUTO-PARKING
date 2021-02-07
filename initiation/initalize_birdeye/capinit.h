extern CvCapture *cap;
void capinit(){
      
      cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_WIDTH, 640);  
      cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_HEIGHT, 480);
      cvSetCaptureProperty( cap, CV_CAP_PROP_FPS, 15);
      cvSetCaptureProperty( cap, CV_CAP_PROP_FRAME_COUNT, 10);
}
