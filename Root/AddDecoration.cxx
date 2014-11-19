#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <xAOD_Decoration_Example/AddDecoration.h>

// count events
#include "xAODEventInfo/EventInfo.h"
// AntiKt10LCTopo Jets
#include "xAODJet/JetContainer.h"
// making shallow copies so we can decorate
#include "xAODCore/ShallowCopy.h"

// this is needed to distribute the algorithm to the workers
ClassImp(AddDecoration)

AddDecoration :: AddDecoration (){};

EL::StatusCode AddDecoration :: setupJob (EL::Job& job)
{
  job.useXAOD ();
  xAOD::Init( "AddDecoration" ).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode AddDecoration :: histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode AddDecoration :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode AddDecoration :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode AddDecoration :: initialize ()
{
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()  ", "Number of events = %lli", m_event->getEntries() ); 
  m_numEvents = 0;
  m_numPassJets = 0;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode AddDecoration :: execute ()
{
  m_numEvents++;

  // retrieve event info - valid events
  const xAOD::EventInfo* eventInfo = 0;
  if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
    Error("execute()  ", "Failed to retrieve event info collection. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // going to change these
  const xAOD::JetContainer* AntiKt10LCTopoJets = 0;
  if ( !m_event->retrieve( AntiKt10LCTopoJets, "AntiKt10LCTopoJets" ).isSuccess() ){
    Error("execute()  ", "Failed to retrieve AntiKt10LCTopoJets container. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // create shallow copy
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > AntiKt10LCTopoJets_shallow = xAOD::shallowCopyContainer( *AntiKt10LCTopoJets );

  // iterate over shallow copy and decorate
  // note: first is the JetContainer in the pair <Container, AuxContainer>
  xAOD::JetContainer::const_iterator jet_itr = (AntiKt10LCTopoJets_shallow.first)->begin();
  xAOD::JetContainer::const_iterator jet_end = (AntiKt10LCTopoJets_shallow.first)->end();
  for( ; jet_itr != jet_end; ++jet_itr ){
    // really trivial example, cast pt as int and store as decorator
    (*jet_itr)->auxdecor<int>("decor_pt") = (int)((*jet_itr)->pt());
    m_numPassJets++;
  }

  if( !m_store->record( AntiKt10LCTopoJets_shallow.first, "DecoratedAntiKt10LCTopoJets" ).isSuccess() ){
    Error("execute()  ", "Failed to store the container. Exiting.");
    return EL::StatusCode::FAILURE;
  }
  if( !m_store->record( AntiKt10LCTopoJets_shallow.second, "DecoratedAntiKt10LCTopoJetsAux." ).isSuccess() ){
    Error("execute()  ", "Failed to store the aux. container. Exiting.");
    return EL::StatusCode::FAILURE;
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode AddDecoration :: postExecute ()
{
  if(m_numEvents % 500 == 0){
    Info("postExecute()  ", "Num Events Processed = %i", m_numEvents);
    Info("postExecute()  ", "Dumping the store");
    m_store->print();
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode AddDecoration :: finalize ()
{
  Info("finalize()  ", "Number of jets decorated = %i", m_numPassJets);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode AddDecoration :: histFinalize () { return EL::StatusCode::SUCCESS; }
