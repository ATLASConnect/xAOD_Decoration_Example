#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "xAOD_Decoration_Example/AddDecoration.h"
#include "xAOD_Decoration_Example/CheckDecoration.h"


int main( int argc, char* argv[] ) {


   // Take the submit directory from the input if provided:
   std::string submitDir = "submitDir";
   if( argc > 1 ) submitDir = argv[ 1 ];

   // Set up the job for xAOD access:
   xAOD::Init().ignore();

   // Construct the samples to run on:
   SH::SampleHandler sh;

   const char* inputFilePath = "/share/t3data/kratsg/xAODs";
   SH::DiskListLocal list (inputFilePath);
   SH::scanDir (sh, list, "AOD.01604209._000001.pool.root.1"); // specifying one particular file for testing

   // Set the name of the input TTree. It's always "CollectionTree"
   // for xAOD files.
   sh.setMetaString( "nc_tree", "CollectionTree" );

   // Print what we found:
   sh.print();

   // Create an EventLoop job:
   EL::Job job;
   job.sampleHandler( sh );

   // Add our analysis to the job:
   AddDecoration * addDecoration = new AddDecoration();
   CheckDecoration * checkDecoration = new CheckDecoration();
   job.algsAdd( addDecoration );
   job.algsAdd( checkDecoration );

   // Run the job using the local/direct driver:
   EL::DirectDriver driver;
   driver.submit( job, submitDir );

   return 0;
}

