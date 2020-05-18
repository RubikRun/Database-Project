#pragma once

#include "../Database/Database.hpp"

class System
{
    //Currently loaded database
    Database* database;

    public:

        //Creates the system with no loaded database
        System();

        //Runs the system
        void Run();

    private:

        //Executes a command
        void ExecuteCommand(const String& command);

        //Executes an open command
        void OpenCommand(const Vector<String>& args);

        //Executes a save command
        void SaveCommand();

        //Executes a save as command
        void SaveAsCommand(const Vector<String>& args);

        //Executes a help command
        void HelpCommand();
};