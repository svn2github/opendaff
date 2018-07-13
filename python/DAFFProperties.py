import sys
import numpy as np

sys.path.append( '../bindings/python/dist/Lib/site-packages' )
import daffCppInterface;

from DAFFOrientation import DAFFOrientation

#The DAFFProperties class contains the properties of the "Property" property    
class DAFFProperties():
# Initialize DAFF-Metadata object
    def __init__(self, index):
        self._propertyDict = daffCppInterface.properties(index);
        self.__alphaPoints = self._propertyDict['AlphaPoints'];
        self.__alphaRange = self._propertyDict['AlphaRange'];
        self.__alphaResolution = self._propertyDict['AlphaResolution'];
        self.__betaPoints = self._propertyDict['BetaPoints'];
        self.__betaRange = self._propertyDict['BetaRange'];
        self.__betaResolution = self._propertyDict['BetaResolution'];
        self.__channelLabels = self._propertyDict['ChannelLabels'];
        self.__contentType = self._propertyDict['ContentType'];
        self.__fileFormatVersion = self._propertyDict['FileFormatVersion'];
        self.__filename = self._propertyDict['Filename'];        
        self.__fullSphere = self._propertyDict['FullSphere'];
        self.__numChannels = self._propertyDict['NumChannels'];        
        self.__numRecords = self._propertyDict['NumRecords'];
        self.__orientation = DAFFOrientation(self._propertyDict['Orientation']);
        self.__orientationDefault = DAFFOrientation(self._propertyDict['OrientationDefault']);
        self.__quantization = self._propertyDict['Quantization'];
        
    # For print(self)    
    def __str__(self):
        return str(self._propertyDict);
    
    # For self representation
    def __repr__(self):
        return repr(self._propertyDict);
    
    # Get the AlphaPoints
    @property
    def AlphaPoints(self):
        return self.__alphaPoints;    
    
    # Get the AlphaRange
    @property
    def AlphaRange(self):
        return self.__alphaRange;
    
    # Get the AlphaResolution
    @property
    def AlphaResolution(self):
        return self.__alphaResolution;
    
    # Get the BetaPoints
    @property
    def BetaPoints(self):
        return self.__betaPoints;

    # Get the BetaRange
    @property
    def BetaRange(self):
        return self.__betaRange;

    # Get the BetaResolution
    @property
    def BetaResolution(self):
        return self.__betaResolution;
    
    # Get the ChannelLabels
    @property
    def ChannelLabels(self):
        return self.__channelLabels;

    # Get the ContentType
    @property
    def ContentType(self):
        return self.__contentType;
        
    # Get the FileFormatVersion
    @property
    def FileFormatVersion(self):
        return self.__fileFormatVersion;
    
    # Get the Filename
    @property
    def Filename(self):
        return self.__filename;
        
    # Get the FullSphere
    @property
    def FullSphere(self):
        return self.__fullSphere;

    # Get the Number of Channels
    @property
    def NumChannels(self):
        return self.__numChannels;
        
    # Get the Number of Records
    @property
    def NumRecords(self):
        return self.__numRecords;
    
    # Get the Orientation
    @property
    def Orientation(self):
        return self.__orientation;
        
    # Get the default orientation
    @property
    def OrientationDefault(self):
        return self.__orientationDefault;
        
    # Get the Quantization
    @property
    def Quantization(self):
        return self.__quantization;

    
    
    
    
#Properties of the Content Type DAFF_IMPULSE_RESPONSE
class DAFFPropertiesIR(DAFFProperties):
    def __init__(self, index):
        DAFFProperties.__init__(self, index);
        self.__samplerate = self._propertyDict['Samplerate'];
        self.__filterLength = self._propertyDict['FilterLength'];    
        
    # Get the Samplerate
    @property
    def Samplerate(self):
        return self.__samplerate;
        
    # Get the FilterLength
    @property
    def FilterLength(self):
        return self.__filterLength;
    



#Properties of the Content Types DAFF_MAGNITUDE_SPECTRUM, DAFF_PHASE_SPECTRUM & DAFF_MAGNITUDE_PHASE_SPECTRUM  
class DAFFPropertiesSpectrum(DAFFProperties):
    def __init__(self, index):
        DAFFProperties.__init__(self, index);
        self.__numFreqs = self._propertyDict['NumFreqs'];
        self.__frequencies = self._propertyDict['Frequencies'];
           
    # Get the number of frequencies
    @property
    def NumFreqs(self):
        return self.__numFreqs;
        
    # Get the Frequency array
    @property
    def Frequencies(self):
        return self.__frequencies;
    
    
    
    
#Properties of the Content Type DAFF_DFT_SPECTRUM 
class DAFFPropertiesDFTSpectrum(DAFFProperties):
    def __init__(self, index):
        DAFFProperties.__init__(self, index);
        self.__transformSize = self._propertyDict['TransformSize'];
        self.__numDFTCoeffs = self._propertyDict['NumDFTCoeffs'];
        self.__isSymmetric = self._propertyDict['IsSymmetric'];
        self.__samplerate = self._propertyDict['Samplerate'];
        self.__frequencyBandwidth = self._propertyDict['FrequencyBandwidth'];
                   
    # Get the TransformSize
    @property
    def TransformSize(self):
        return self.__transformSize;
        
    # Get the number of DFT Coefficients
    @property
    def NumDFTCoeffs(self):
        return self.__numDFTCoeffs;      
    
    # Get the IsSymmetric value
    @property
    def IsSymmetric(self):
        return self.__isSymmetric;
        
    # Get the Samplerate
    @property
    def Samplerate(self):
        return self.__samplerate;
                
    # Get the FrequencyBandwidth
    @property
    def FrequencyBandwidth(self):
        return self.__frequencyBandwidth;
      