#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <xAOD_Decoration_Example/CheckDecoration.h>

// count events
#include "xAODEventInfo/EventInfo.h"
// AntiKt10LCTopo Jets
#include "xAODJet/JetContainer.h"

// this is needed to distribute the algorithm to the workers
ClassImp(CheckDecoration)



CheckDecoration :: CheckDecoration (){};

EL::StatusCode CheckDecoration :: setupJob (EL::Job& job)
{
  job.useXAOD ();
  xAOD::Init( "CheckDecoration" ).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode CheckDecoration :: histInitialize ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode CheckDecoration :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode CheckDecoration :: changeInput (bool firstFile)
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode CheckDecoration :: initialize ()
{
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events = %lli", m_event->getEntries() ); 
  m_numEvents = 0;
  m_numPassJets = 0;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode CheckDecoration :: execute ()
{
  m_numEvents++;

  const xAOD::EventInfo* eventInfo = 0;
  if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
    Error("execute()", "Failed to retrieve event info collection. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // I don't use this because I can't loop because i suck at this
  /*
  const xAOD::JetContainer* AntiKt10LCTopoJets = 0;
  if ( !m_event->retrieve( AntiKt10LCTopoJets, "AntiKt10LCTopoJets" ).isSuccess() ){
    Error("execute()", "Failed to retrieve AntiKt10LCTopoJets container. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  */

  const xAOD::JetContainer* AntiKt10LCTopoJets_shallow = 0;
  if( !m_store->retrieve( AntiKt10LCTopoJets_shallow, "DecoratedAntiKt10LCTopoJets" ).isSuccess() ){
    Error("execute()", "Failed to retrieve decorated AntiKt10LCTopoJets container. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // cache name-lookup in a loop
  SG::AuxElement::Decorator<int> decor_pt ("decor_pt");

  // how the mother fuck can i loop using the following?!?!
  /*
  for(unsigned i = 0; i < (*AntiKt10LCTopoJets).size(); i++){
    if(decor_pt.isAvailable( AntiKt10LCTopoJets )){
      int expectValue = decor_pt(*AntiKt10LCTopoJets_shallow[i]); // double-cast, auto promotion???
      int actualValue = (int)((*AntiKt10LCTopoJets[i]).pt());
      if( expectValue != actualValue){
        Error("execute()", "Decoration value incorrect. Expected %i, got %i", expectValue, actualValue);
      } else {
        // correct, increment
        m_numPassJets++;
      }
    } else {
      Error("execute()", "Failed to find decoration");
    }
  }
  */

  xAOD::JetContainer::const_iterator jet_itr = AntiKt10LCTopoJets_shallow->begin();
  xAOD::JetContainer::const_iterator jet_end = AntiKt10LCTopoJets_shallow->end();
  for( ; jet_itr != jet_end; ++jet_itr ){
    if(decor_pt.isAvailable( **jet_itr )){
      int expectValue = decor_pt(**jet_itr); // double-cast, auto promotion???
      int actualValue = (int)((*jet_itr)->pt());
      if( expectValue != actualValue){
        Error("Cxecute()", "Decoration value incorrect. Expected %i, got %i", expectValue, actualValue);
      } else {
        // correct, increment
        m_numPassJets++;
      }
    } else {
      Error("CheckDecoration::execute()", "Failed to find decoration");
    }
  }


  return EL::StatusCode::SUCCESS;
}

EL::StatusCode CheckDecoration :: postExecute () { 
  if(m_numEvents % 500 == 0){
    Info("postExecute()", "Num Events Processed = %i", m_numEvents);
    Info("postExecute()", "Clearing the store");
    m_store->clear();
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode CheckDecoration :: finalize ()
{
  Info("finalize()", "Number of decorated jets = %i", m_numPassJets);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode CheckDecoration :: histFinalize () { return EL::StatusCode::SUCCESS; }
