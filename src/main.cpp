#include "includes.h"
#include "Simulation.h"
#include <unistd.h>

void printUsage() {
   std::cout << "Usage: ./dynamic-pathing -g <graphFile> -a <agentFile> -d <debug level> " << std::endl;
}

int main(int argc, char **argv) {
   // * Process the command line arguments
   LOGGING_LEVEL = 0;
   std::string graphFile = "";
   std::string agentFile = "";
   const std::string DEFAULT_OUTPUT = "outputs/output.txt";
   std::string outputFile = DEFAULT_OUTPUT;

   // * Check the Arguments
   int option;
   while((option = getopt(argc, argv, "d:g:a:o:")) != -1) {
      switch(option) {
         case 'g':
            graphFile = optarg;
            break;
         case 'a':
            agentFile = optarg;
            break;
         case 'o':
            outputFile = optarg;
            break;
         case 'd':
            LOGGING_LEVEL = atoi(optarg);
            break;
         case ':':
         case '?':
         default:
            printUsage();
            exit(-1);
         }
   }
   if ( graphFile == "" || agentFile == "" ) {
      ERROR << "Must provide a graph file and an agent file." << ENDL;
      printUsage();
   }
   if ( outputFile == DEFAULT_OUTPUT ) {
      INFO << "No output path provided... Will output results to output/output.txt instead." << ENDL;
   }
   INFO << "Running Search on files " << graphFile << " and " << agentFile << ENDL;
   INFO << "Debug Level: " << LOGGING_LEVEL << ENDL;

   // * Set up the Simulation 
   Simulation sim(graphFile, agentFile, outputFile);
   sim.runSearch();

   return 0;
}