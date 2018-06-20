#The DAFFOrientation class contains the orientation angles used by the DAFFProperties class
class DAFFOrientation():
    def __init__(self, orientationDict):
        self.__orientationDict = orientationDict;
        self.__pitchAngle = orientationDict['PitchAngle'];
        self.__rollAngle = orientationDict['RollAngle'];
        self.__yawAngle = orientationDict['YawAngle'];
        
        
    # For print(self)    
    def __str__(self):
        return str(self.__orientationDict);
    
    # For self representation
    def __repr__(self):
        return repr(self.__orientationDict);


    # Get the PitchAngle
    @property
    def PitchAngle(self):
        return self.__pitchAngle;
        
    # Get the RollAngle
    @property
    def RollAngle(self):
        return self.__rollAngle;
                
    # Get the YawAngle
    @property
    def YawAngle(self):
        return self.__yawAngle;


