
#include <Arduino.h>
#include <SimpleCLI.h>

#include "eps-cli.h"

SimpleCLI cli;

// All commands should be declared bellow
Command helpCommand;
Command testCommand;


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
display information about all the supported commands
*/
void helpCommandCallback(cmd* commandPointer) {
  Command cmd(commandPointer); // Create wrapper class instance for the pointer
  
  Serial.println("supported commands :");
  Serial.println("\"help\" : Show the list of supported commands.");
  Serial.println("\"test\" : Dummy command used as a template.");
}


/*
testCommandCallback
this is just a template for developing the other commands
*/
void testCommandCallback(cmd* commandPointer) {
  Command cmd(commandPointer); // Create wrapper class instance for the pointer
  
  Serial.println("test command sucessfull");
}


/*
setupEpsCli
this should be called only once in setup() function of the main file to make sure the cli is initialized
*/
void setupEpsCli(){
    Serial.begin(115200);

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
    Serial.println("type \"help\" for a list of supported commands");
    Serial.println("");

    Serial.setTimeout(1);
    Serial.printf("> "); // Command prompt

    cli.setOnError(errorCallback); // Set error Callback

    // Add all commands to the cli class and associate their callback functions bellow.
    // 
    helpCommand = cli.addCommand("help", helpCommandCallback);
    testCommand = cli.addCommand("test", testCommandCallback);
}


/*
manageEpsCli
this should be called in the loop() function of the main file to make sure all received commands are parsed and executed.
*/
void manageEpsCli(){
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
