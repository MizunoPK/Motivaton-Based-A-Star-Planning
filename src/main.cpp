#include "includes.h"
#include <iostream>
#include "Simulation.h"

void printUsage() {
   std::cout << "Usage: ./dynamic-pathing {nodesFile} {adjacenciesFile} {agentFile}" << std::endl;
}

int main(int argc, char **argv) {
   // * Process the command line arguments
   boost::log::add_console_log(std::cout, boost::log::keywords::format = "%Message%");
   boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);

   // * Check the Arguments
   int option;
   while((option = getopt(argc, argv, "v")) != -1) {
      switch(option){
         case 'v':
            std::cout << "-v detected" << std::endl;
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
            break;
         case ':':
         case '?':
         default:
            std::cout << "usage: " << argv[0] << " -v" << std::endl;
            exit(-1);
         }
      }

   // * Get the file names
   std::string nodesFile = argv[optind++];
   std::string adjacenciesFile = argv[optind++];
   std::string agentFile = argv[optind];

   // * Set up the Simulation 
   Simulation sim(nodesFile, adjacenciesFile, agentFile);

   return 0;
}