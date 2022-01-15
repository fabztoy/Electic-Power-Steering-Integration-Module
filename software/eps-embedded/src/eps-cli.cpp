
#include <Arduino.h>
#include <SimpleCLI.h>

#include "eps-cli.h"
#include "eps-parameters.h"

#include "speedometer.h"

SimpleCLI cli;

// All commands should be declared bellow
Command helpCommand;
Command testCommand;
Command catcfCommand;
Command rmcfCommand;
Command reloadcfCommand;
Command savecfCommand;
Command restorecfCommand;
Command updatepCommand;
Command rebootCommand;


extern void loadParametersFromFile();
extern void saveParametersToFile();
extern void restoreEpsConfigFile();


/*

*/
void printSplash(){
    // Draw splash screen
    // this is generated from https://patorjk.com/software/taag/
    // https://patorjk.com/software/taag/#p=display&f=Big&t=EPS
    Serial.println("  ______ _____   _____          _   _ ");
    Serial.println(" |  ____|  __ \\ / ____|        | | (_)");
    Serial.println(" | |__  | |__) | (___     ___  | |  _ ");
    Serial.println(" |  __| |  ___/ \\___ \\   / __| | | | |");
    Serial.println(" | |____| |     ____) | | (__  | | | |");
    Serial.println(" |______|_|    |_____/   \\___| |_| |_|");
    Serial.println("");
    Serial.println("https://github.com/fabztoy/Electic-Power-Steering-Integration-Module");
    Serial.println("");
}


/*
errorCallback
this callback is called everytime there is an error parsing the cli commands
*/
void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}


/*
helpCommandCallback
* Display information about all the supported commands.
*/
void helpCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    Serial.println("_______________________________________________________________________________");
    Serial.println("supported commands :");
    Serial.println("===============================================================================");
    Serial.println("help                    : Display information about all the supported commands.");
    Serial.println("test                    : Dummy command used as a template.");
    Serial.println("catcf                   : print the whole config file");
    Serial.println("rmcf                    : delete the whole config file");
    Serial.println("reloadcf                : reload the whole config file");
    Serial.println("savecf                  : save the current config into the config file");
    Serial.println("restorecf               : restore the config file to what it was before saving");
    Serial.println("updatep PNAME PVALUE    : update the parameter PNAME to PVALUE");
    Serial.println("reboot                  : reboot");
    Serial.println("______________________________________________________________________________");
    Serial.println("Remember that backspace is not supported");
    Serial.println("______________________________________________________________________________");
}


/*
testCommandCallback
* Dummy command used as a template.
*/
void testCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    Serial.println("test command sucessfull");
}


/*
catcfCommandCallback
* print the whole config file.
*/
void catcfCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    printEpsConfigFile();
}


/*
rmcfCommandCallback
* delete the whole config file.
*/
void rmcfCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    deleteEpsConfigFile();
}


/*
reloadcfCommandCallback
* reload the whole config file.
*/
void reloadcfCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    loadParametersFromFile();

    speedometerInit();
}


/*
savecfCommandCallback
* save the current config into the config file.
*/
void savecfCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    saveParametersToFile();
}


/*
restorecfCommandCallback
* restore the config file to what it was before saving.
*/
void restorecfCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    restoreEpsConfigFile();
}


/*
updatepCommandCallback
*/
void updatepCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    updateEpsParameter(cmd.getArgument("PNAME").getValue(), cmd.getArgument("PVALUE").getValue());
}


/*
rebootCommandCallback
* reboot mcu.
*/
void rebootCommandCallback(cmd* commandPointer) {
    Command cmd(commandPointer); // Create wrapper class instance for the pointer
    ESP.restart();
}


/*
setupEpsCli
this should be called only once in setup() function of the main file to make sure the cli is initialized
*/
void epsCliInit(){
    Serial.begin(115200);
    printSplash();
    Serial.println("type \"help\" for a list of supported commands");
    Serial.println("");

    Serial.setTimeout(1);
    Serial.printf("> "); // Command prompt

    cli.setOnError(errorCallback); // Set error Callback

    // Add all commands to the cli class and associate their callback functions bellow.
    // 
    helpCommand = cli.addCommand("help", helpCommandCallback);
    testCommand = cli.addCommand("test", testCommandCallback);
    catcfCommand = cli.addCommand("catcf", catcfCommandCallback);
    rmcfCommand = cli.addCommand("rmcf", rmcfCommandCallback);
    reloadcfCommand = cli.addCommand("reloadcf", reloadcfCommandCallback);
    savecfCommand = cli.addCommand("savecf", savecfCommandCallback);
    restorecfCommand = cli.addCommand("restorecf", restorecfCommandCallback);
    updatepCommand = cli.addCommand("updatep", updatepCommandCallback);
        updatepCommand.addPositionalArgument("PNAME");
        updatepCommand.addPositionalArgument("PVALUE");
    rebootCommand = cli.addCommand("reboot", rebootCommandCallback);

}


/*
manageEpsCli
this should be called in the loop() function of the main file to make sure all received commands are parsed and executed.
*/
void epsCliManage(){
    static String input = "";

    if (Serial.available()){
        String latestData = Serial.readString();
        
        if(latestData.indexOf('\n') != -1){
            // If newline (enter key) is found

            // concatenate the latest data (up to the newline)
            input = input + latestData.substring(0,latestData.indexOf('\n'));

            // keep only the remaining non parsed data
            latestData = latestData.substring(latestData.indexOf('\n')+1, latestData.length());

            Serial.println("# " + input);   // Display the Command
            cli.parse(input);               // Parse and execute the command

            Serial.println("");
            Serial.printf("> ");    // Command prompt
            input = latestData;     // keep the remaining non parsed data in input

        }else{
            // If newline is not detected
            // concatenate all the latest data then clear it
            input = input + latestData;
            latestData = "";
        }
    }
}
