import sys
import numpy as np

sys.path.append( '../bindings/python/dist/Lib/site-packages' )
import daffCppInterface;

from DAFFProperties import DAFFPropertiesIR
from DAFFProperties import DAFFPropertiesSpectrum
from DAFFProperties import DAFFPropertiesDFTSpectrum
from DAFFMetadata import DAFFMetadata


# The class for the OpenDAFF objects
class DAFF():
    # Initialize DAFF object 
    def __init__(self, filepath):
        self._index = daffCppInterface.open(filepath);
        self.view = 1;
        self.__contentType = daffCppInterface.content_type(self._index);
        self.__contentTypeString = daffCppInterface.content_type_str(self._index);
        
        self.__properties = daffCppInterface.properties(self._index);
        self.__metadata = daffCppInterface.metadata(self._index);
        
        if (self.__contentType == 0): # contentType == DAFF_IMPULSE_RESPONSE
            self.__properties = DAFFPropertiesIR(self._index);
        elif (self.__contentType == 4): # contentType == DAFF_DFT_SPECTRUM
            self.__properties = DAFFPropertiesDFTSpectrum(self._index);
        else:
            self.__properties = DAFFPropertiesSpectrum(self._index);
            
        
    
    # Get the nearest neighbour of the current position   
    def GetNearestNeighbour(self,angle1,angle2):
        return daffCppInterface.nearest_neighbour_index(self._index,self.view, angle1,angle2);
    
    # Get the index of the nearest neighbour of the current position   
    def GetNearestNeighbourIndex(self,angle1,angle2):
        return daffCppInterface.nearest_neighbour_index(self._index,self.view, angle1,angle2)[0];
    
    # Check if the nearest neighbour is out of bounds
    def IsNearestNeighbourOutOfBounds(self,angle1,angle2):
        return daffCppInterface.nearest_neighbour_index(self._index,self.view, angle1,angle2)[1];
     
    # Get the records of the nearest neighbour of the current position   
    def GetNearestNeighbourRecord(self,angle1,angle2):
        return daffCppInterface.nearest_neighbour_record(self._index,self.view, angle1,angle2);    
    
    # Get the records of the nearest neighbour of the current position   
    def GetRecord(self,index):
        return daffCppInterface.record(self._index,index);    
    
    # Get the content type
    @property
    def ContentType(self):
        return self.__contentType;
    
    # Get the content type string
    @property
    def ContentTypeString(self):
        return self.__contentTypeString;
    
    # Get the properties
    @property
    def Properties(self):
        return self.__properties;

    # Get the metadata
    @property
    def Metadata(self):
        return self.__metadata;
    


        













