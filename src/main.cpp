#include <iostream>
#include "Simulation.h"

void printUsage() {
   std::cout << "Usage: ./dynamic-pathing {nodesFile} {adjacenciesFile} {agentFile}" << std::endl;
}

int main(int argc, char **argv) {
   // Check the Arguments
   if (argc != 4) {
      std::cout << "Error: 3 command line arguments expected. Recieved " << argc - 1 << std::endl;
      printUsage();
      exit(-1);
   }

   // Get the file names
   std::string nodesFile = argv[1];
   std::string adjacenciesFile = argv[2];
   std::string agentFile = argv[3];

   // Set up the Simulation 
   Simulation sim(nodesFile, adjacenciesFile, agentFile);

   return 0;
}