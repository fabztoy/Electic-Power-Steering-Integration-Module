#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include <eps-parameters.h>

#define FORMAT_SPIFFS_IF_FAILED true

#define EPS_CONFIG_FILE_NAME "/systemconfig.JSON"
#define EPS_BACKUP_CONFIG_FILE_NAME "/systemconfig_1.JSON"

EpsConfig epsConfig;
EpsConfig epsDefaultConfig={

    {   // EpsTyreConfig P_tyreConfig
        205,    // uint16_t P_tyreWidth;   // Tyre Width in mm
        50,     // uint8_t P_tyreAR;       // Tyre aspect Ratio in %
        15      // uint8_t P_wheelSize_in; // Wheel size in Inches
    },
    4,          // uint8_t P_vssPulsesPerRev,  // vss pulses per revolutions
    100,        // uint8_t P_diffToVssRatio,   // vss to differential ratio in %

    {   // EpsSpeedometerConfig P_speedometerConfig
        10.616, // float P_speedometerSpeedConstant;           // speedometer speed constant in rpm/kph
        28,     // uint16_t P_speedometerMotorStepsPerRev;     // motor steps per revolution (360/StepAngle)
        4,      // uint16_t P_speedometerMotorCurrentRating;   // motor current rating in mA
        100     // uint16_t P_speedometerMotorGearboxRatio;    // motor gear ratio in %
    },
    
    {   // EpsSpeedLookupConfig P_speedLookupConfig;
    20,         // uint8_t P_speedLookupPt1_X;
    20,         // uint8_t P_speedLookupPt1_Y;
    50,         // uint8_t P_speedLookupPt2_X;
    50,         // uint8_t P_speedLookupPt2_Y;
    100,        // uint8_t P_speedLookupPt3_X;
    100,        // uint8_t P_speedLookupPt3_Y;
    110,        // uint8_t P_speedLookupPt4_X;
    110,        // uint8_t P_speedLookupPt4_Y;
    150,        // uint8_t P_speedLookupPt5_X;
    150,        // uint8_t P_speedLookupPt5_Y;
    }
};


void loadDefaultParameters();
void saveParametersToFile();
void loadParametersFromFile();
void restoreEpsConfigFile();


/*
epsParametersInit
* Initialize the SPIFFS file system and mount it.
* Load parameters from file
8 If the file does not exist or the FS fails to mount, default parameters will be loaded.
*/
void epsParametersInit(){
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");

        /*
        Raise an error
        load_default_parameters()
        */
    }

    loadParametersFromFile();
}


/*
loadDefaultParameters
* Simply load epsDefaultConfig into epsConfig
*/
void loadDefaultParameters(){
    epsConfig = epsDefaultConfig;
}


/*
loadParametersFromFile
* load all parameters from file
* if the file does not exist, defaults are loaded and saved.
* if a parameter is missing, the default value is loaded.
*/
void loadParametersFromFile(){

    if (!SPIFFS.exists(EPS_CONFIG_FILE_NAME)){
        /*
        parameter file does not exist, will load default and create the file
        */
        loadDefaultParameters();
        saveParametersToFile();
        return;
    }

    File f = SPIFFS.open(EPS_CONFIG_FILE_NAME);

    StaticJsonDocument<1024> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, f);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));

    // Copy values from the JsonDocument to the Config
    epsConfig.P_tyreConfig.P_tyreWidth = doc["P_tyreConfig"]["P_tyreWidth"] | epsDefaultConfig.P_tyreConfig.P_tyreWidth;
    epsConfig.P_tyreConfig.P_tyreAR = doc["P_tyreConfig"]["P_tyreAR"] | epsDefaultConfig.P_tyreConfig.P_tyreAR;
    epsConfig.P_tyreConfig.P_wheelSize_in = doc["P_tyreConfig"]["P_wheelSize_in"] | epsDefaultConfig.P_tyreConfig.P_wheelSize_in;

    epsConfig.P_vssPulsesPerRev = doc["P_vssPulsesPerRev"] | epsDefaultConfig.P_vssPulsesPerRev;
    epsConfig.P_diffToVssRatio = doc["P_diffToVssRatio"] | epsDefaultConfig.P_diffToVssRatio;
    
    epsConfig.P_speedometerConfig.P_speedometerSpeedConstant = doc["P_speedometerConfig"]["P_speedometerSpeedConstant"] | epsDefaultConfig.P_speedometerConfig.P_speedometerSpeedConstant;
    epsConfig.P_speedometerConfig.P_speedometerMotorStepsPerRev = doc["P_speedometerConfig"]["P_speedometerMotorStepsPerRev"] | epsDefaultConfig.P_speedometerConfig.P_speedometerMotorStepsPerRev;
    epsConfig.P_speedometerConfig.P_speedometerMotorCurrentRating = doc["P_speedometerConfig"]["P_speedometerMotorCurrentRating"] | epsDefaultConfig.P_speedometerConfig.P_speedometerMotorCurrentRating;
    epsConfig.P_speedometerConfig.P_speedometerMotorGearboxRatio = doc["P_speedometerConfig"]["P_speedometerMotorGearboxRatio"] | epsDefaultConfig.P_speedometerConfig.P_speedometerMotorGearboxRatio;
    
    epsConfig.P_speedLookupConfig.P_speedLookupPt1_X = doc["P_speedLookupConfig"]["P_speedLookupPt1_X"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt1_X;
    epsConfig.P_speedLookupConfig.P_speedLookupPt1_Y = doc["P_speedLookupConfig"]["P_speedLookupPt1_Y"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt1_Y;
    epsConfig.P_speedLookupConfig.P_speedLookupPt2_X = doc["P_speedLookupConfig"]["P_speedLookupPt2_X"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt2_X;
    epsConfig.P_speedLookupConfig.P_speedLookupPt2_Y = doc["P_speedLookupConfig"]["P_speedLookupPt2_Y"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt2_Y;
    epsConfig.P_speedLookupConfig.P_speedLookupPt3_X = doc["P_speedLookupConfig"]["P_speedLookupPt3_X"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt3_X;
    epsConfig.P_speedLookupConfig.P_speedLookupPt3_Y = doc["P_speedLookupConfig"]["P_speedLookupPt3_Y"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt3_Y;
    epsConfig.P_speedLookupConfig.P_speedLookupPt4_X = doc["P_speedLookupConfig"]["P_speedLookupPt4_X"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt4_X;
    epsConfig.P_speedLookupConfig.P_speedLookupPt4_Y = doc["P_speedLookupConfig"]["P_speedLookupPt4_Y"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt4_Y;
    epsConfig.P_speedLookupConfig.P_speedLookupPt5_X = doc["P_speedLookupConfig"]["P_speedLookupPt5_X"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt5_X;
    epsConfig.P_speedLookupConfig.P_speedLookupPt5_Y = doc["P_speedLookupConfig"]["P_speedLookupPt5_Y"]  | epsDefaultConfig.P_speedLookupConfig.P_speedLookupPt5_Y;

    if (error)
        saveParametersToFile();
    
    // Close the file (Curiously, File's destructor doesn't close the file)
    f.close();
}


/*
saveParametersToFile
* serialize all values to the json config file
*/
void saveParametersToFile(){
    // Delete existing file, otherwise the configuration is appended to the file
    if (SPIFFS.exists(EPS_BACKUP_CONFIG_FILE_NAME)){
        SPIFFS.remove(EPS_BACKUP_CONFIG_FILE_NAME);
    }

    SPIFFS.rename(EPS_CONFIG_FILE_NAME, EPS_BACKUP_CONFIG_FILE_NAME);

    // Open file for writing
    File f = SPIFFS.open(EPS_CONFIG_FILE_NAME, FILE_WRITE);
    if (!f) {
        Serial.println(F("Failed to create file"));
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use https://arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<1024> doc;

    // Set the values in the document
    doc["P_tyreConfig"]["P_tyreWidth"] = epsConfig.P_tyreConfig.P_tyreWidth;
    doc["P_tyreConfig"]["P_tyreAR"] = epsConfig.P_tyreConfig.P_tyreAR;
    doc["P_tyreConfig"]["P_wheelSize_in"] = epsConfig.P_tyreConfig.P_wheelSize_in;

    doc["P_vssPulsesPerRev"] = epsConfig.P_vssPulsesPerRev;
    doc["P_diffToVssRatio"] = epsConfig.P_diffToVssRatio;
    
    doc["P_speedometerConfig"]["P_speedometerSpeedConstant"] = epsConfig.P_speedometerConfig.P_speedometerSpeedConstant;
    doc["P_speedometerConfig"]["P_speedometerMotorStepsPerRev"] = epsConfig.P_speedometerConfig.P_speedometerMotorStepsPerRev;
    doc["P_speedometerConfig"]["P_speedometerMotorCurrentRating"] = epsConfig.P_speedometerConfig.P_speedometerMotorCurrentRating;
    doc["P_speedometerConfig"]["P_speedometerMotorGearboxRatio"] = epsConfig.P_speedometerConfig.P_speedometerMotorGearboxRatio;
    
    doc["P_speedLookupConfig"]["P_speedLookupPt1_X"] = epsConfig.P_speedLookupConfig.P_speedLookupPt1_X;
    doc["P_speedLookupConfig"]["P_speedLookupPt1_Y"] = epsConfig.P_speedLookupConfig.P_speedLookupPt1_Y;
    doc["P_speedLookupConfig"]["P_speedLookupPt2_X"] = epsConfig.P_speedLookupConfig.P_speedLookupPt2_X;
    doc["P_speedLookupConfig"]["P_speedLookupPt2_Y"] = epsConfig.P_speedLookupConfig.P_speedLookupPt2_Y;
    doc["P_speedLookupConfig"]["P_speedLookupPt3_X"] = epsConfig.P_speedLookupConfig.P_speedLookupPt3_X;
    doc["P_speedLookupConfig"]["P_speedLookupPt3_Y"] = epsConfig.P_speedLookupConfig.P_speedLookupPt3_Y;
    doc["P_speedLookupConfig"]["P_speedLookupPt4_X"] = epsConfig.P_speedLookupConfig.P_speedLookupPt4_X;
    doc["P_speedLookupConfig"]["P_speedLookupPt4_Y"] = epsConfig.P_speedLookupConfig.P_speedLookupPt4_Y;
    doc["P_speedLookupConfig"]["P_speedLookupPt5_X"] = epsConfig.P_speedLookupConfig.P_speedLookupPt5_X;
    doc["P_speedLookupConfig"]["P_speedLookupPt5_Y"] = epsConfig.P_speedLookupConfig.P_speedLookupPt5_Y;

    // Serialize JSON to file
    if (serializeJsonPretty(doc, f) == 0) {
        Serial.println(F("Failed to write to file"));
    }

    // Close the file
    f.close();
}


void updateUint16Parameter(String pvalue, uint16_t* pparameter){
    int myInt(pvalue.toInt());

    if (myInt <= static_cast<int>(UINT16_MAX) && myInt >=0) {
        *pparameter = static_cast<uint16_t>(myInt);
        Serial.printf("updated to %i\n", *pparameter);
    }else {
        Serial.println("Error : could not cast to utint16_t");
    }
}


void updateUint8Parameter(String pvalue, uint8_t* pparameter){
    int myInt(pvalue.toInt());

    if (myInt <= static_cast<int>(UINT8_MAX) && myInt >=0) {
        *pparameter = static_cast<uint8_t>(myInt);
        Serial.printf("updated to %i\n", *pparameter);
    }else {
        Serial.println("Error : could not cast to utint8_t");
    }
}


void updateFloatParameter(String pvalue, float* pparameter){
    *pparameter = pvalue.toFloat();
    Serial.printf("updated to %f\n", *pparameter);
}


/*
updateEpsParameter

*/
void updateEpsParameter(String pname, String pvalue){
    bool success = true;
    Serial.printf("parameter %s ", pname.c_str());
    if (pname == "P_tyreWidth"){
        // uint16_t epsConfig.P_tyreConfig.P_tyreWidth
        updateUint16Parameter(pvalue, &(epsConfig.P_tyreConfig.P_tyreWidth));
    }else if(pname == "P_tyreAR"){
        // uint8_t epsConfig.P_tyreConfig.P_tyreAR
        updateUint8Parameter(pvalue, &(epsConfig.P_tyreConfig.P_tyreAR));
    }else if(pname == "P_wheelSize_in"){
        // uint8_t epsConfig.P_tyreConfig.P_wheelSize_in
        updateUint8Parameter(pvalue, &(epsConfig.P_tyreConfig.P_wheelSize_in));
    }else if(pname == "P_vssPulsesPerRev"){
        // uint8_t epsConfig.P_vssPulsesPerRev
        updateUint8Parameter(pvalue, &(epsConfig.P_vssPulsesPerRev));
    }else if(pname == "P_diffToVssRatio"){
        // uint8_t epsConfig.P_diffToVssRatio
        updateUint8Parameter(pvalue, &(epsConfig.P_diffToVssRatio));
    }else if(pname == "P_speedometerSpeedConstant"){
        // float epsConfig.P_speedometerConfig.P_speedometerSpeedConstant
        updateFloatParameter(pvalue, &(epsConfig.P_speedometerConfig.P_speedometerSpeedConstant));
    }else if(pname == "P_speedometerMotorStepsPerRev"){
        // uint16_t epsConfig.P_speedometerConfig.P_speedometerMotorStepsPerRev;
        updateUint16Parameter(pvalue, &(epsConfig.P_speedometerConfig.P_speedometerMotorStepsPerRev));
    }else if(pname == "P_speedometerMotorCurrentRating"){
        // uint16_t epsConfig.P_speedometerConfig.P_speedometerMotorCurrentRating;
        updateUint16Parameter(pvalue, &(epsConfig.P_speedometerConfig.P_speedometerMotorCurrentRating));
    }else if(pname == "P_speedometerMotorGearboxRatio"){
        // uint16_t epsConfig.P_speedometerConfig.P_speedometerMotorGearboxRatio;
        updateUint16Parameter(pvalue, &(epsConfig.P_speedometerConfig.P_speedometerMotorGearboxRatio));
    }else if(pname == "P_speedLookupPt1_X"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt1_X
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt1_X));
    }else if(pname == "P_speedLookupPt1_Y"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt1_Y
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt1_Y));
    }else if(pname == "P_speedLookupPt2_X"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt2_X
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt2_X));
    }else if(pname == "P_speedLookupPt2_Y"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt2_Y
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt2_Y));
    }else if(pname == "P_speedLookupPt3_X"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt3_X
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt3_X));
    }else if(pname == "P_speedLookupPt3_Y"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt3_Y
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt3_Y));
    }else if(pname == "P_speedLookupPt4_X"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt4_X
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt4_X));
    }else if(pname == "P_speedLookupPt4_Y"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt4_Y
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt4_Y));
    }else if(pname == "P_speedLookupPt5_X"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt5_X
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt5_X));
    }else if(pname == "P_speedLookupPt5_Y"){
        // uint8_t epsConfig.P_speedLookupConfig.P_speedLookupPt5_Y
        updateUint8Parameter(pvalue, &(epsConfig.P_speedLookupConfig.P_speedLookupPt5_Y));
    }else{
        // Unsupported parameter.
        Serial.printf("is not found in \"void updateEpsParameter(\" in \"eps-parameters.cpp\"\n");
        success = false;
    }

    if (success){
        Serial.println("do not forget to usecommand savecf and then reboot to make sure that all parameters are reloaded correctly.");
    }
}

/*
printEpsConfigFile
* Print the config file trough Serial prints.
*/
void printEpsConfigFile(){
    File f = SPIFFS.open(EPS_CONFIG_FILE_NAME);

    if (!f){
       /*
       parameter file does not exist, will load default and create the file
       */
       Serial.println("% Error, file does not exist.");
       return;
    }

    while(f.available()){
        Serial.write(f.read());
    }
}


/*
deleteEpsConfigFile
* delete the config file.
*/
void deleteEpsConfigFile(){
    SPIFFS.remove(EPS_CONFIG_FILE_NAME);
}


/*
restoreEpsConfigFile
* swap EPS_BACKUP_CONFIG_FILE_NAME and EPS_CONFIG_FILE_NAME
* load the new EPS_CONFIG_FILE_NAME
*/
void restoreEpsConfigFile(){

    if (!SPIFFS.exists(EPS_BACKUP_CONFIG_FILE_NAME)){
        // backup parameter file does not exist, load default and create the file
        Serial.println("No backup config exists, will use defaults.");
        SPIFFS.rename(EPS_CONFIG_FILE_NAME, EPS_BACKUP_CONFIG_FILE_NAME);
        loadDefaultParameters();
        saveParametersToFile();
        Serial.println("Default parameters loaded.");
        return;
    }

    // switch both files EPS_CONFIG_FILE_NAME and EPS_BACKUP_CONFIG_FILE_NAME
    if (SPIFFS.exists(EPS_CONFIG_FILE_NAME)){
        SPIFFS.rename(EPS_CONFIG_FILE_NAME, "/temp");
        SPIFFS.rename(EPS_BACKUP_CONFIG_FILE_NAME, EPS_CONFIG_FILE_NAME);
        SPIFFS.rename("/temp", EPS_BACKUP_CONFIG_FILE_NAME);
    }else{
        SPIFFS.rename(EPS_BACKUP_CONFIG_FILE_NAME, EPS_CONFIG_FILE_NAME);
    }
    
    loadParametersFromFile();

    Serial.println("Parameters restored from last save.");
}