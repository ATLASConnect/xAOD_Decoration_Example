#ifndef xAOD_Decoration_Example_AddDecoration_H
#define xAOD_Decoration_Example_AddDecoration_H

#include <EventLoop/Algorithm.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

class AddDecoration : public EL::Algorithm
{
public:
  xAOD::TEvent * m_event; //!
  xAOD::TStore * m_store; //!
  int m_numEvents; //!
  int m_numPassJets; //!

  // this is a standard constructor
  AddDecoration ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(AddDecoration, 1);
};

#endif
