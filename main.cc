//g++ -o main main.cc Track.cc Hit.cc `root-config --libs --cflags`

#include <iostream>
#include "Track.h"
#include "Matrix.h"
#include "KalmanUtils.h"
#include "Propagation.h"

int main() {

  SMatrix36 projMatrix36;
  projMatrix36(0,0)=1.;
  projMatrix36(1,1)=1.;
  projMatrix36(2,2)=1.;
  //std::cout << "projMatrix36" << std::endl;
  //dumpMatrix(projMatrix36);
  SMatrix63 projMatrix36T = ROOT::Math::Transpose(projMatrix36);
  //std::cout << "projMatrix36T" << std::endl;
  //dumpMatrix(projMatrix36T);

  Point pos;
  Vector mom;
  SMatrixSym66 covtrk;
  std::vector<Hit> hits;
  setupTrackByHand(pos,mom,covtrk,hits,0);
  int q = 1;
  Track trk(q,pos,mom,covtrk);
  trk.setHitsVector(hits);

  std::cout << "init x: " << pos.x() << " " << pos.y() << " " << pos.z() << std::endl;
  std::cout << "init p: " << trk.momentum().x() << " " << trk.momentum().y() << " " << trk.momentum().z() << std::endl;
  std::cout << "init e: " << std::endl;
  dumpMatrix(covtrk);

  TrackState initState = trk.state();

  TrackState tmpInitState = initState;

  for (std::vector<Hit>::iterator hit=hits.begin();hit!=hits.end();++hit) {

    std::cout << std::endl;
    std::cout << "processing hit #" << hit-hits.begin() << std::endl;

    TrackState propState = propagateLineToR(tmpInitState,hit->position().Rho());

    std::cout << "propState.parameters" << std::endl;
    std::cout << "x: " << propState.parameters[0] << " " << propState.parameters[1] << " " << propState.parameters[2] << std::endl;
    std::cout << "p: " << propState.parameters[3] << " " << propState.parameters[4] << " " << propState.parameters[5] << std::endl;
    std::cout << "propState.errors" << std::endl;
    dumpMatrix(propState.errors);
    
    MeasurementState measState = hit->measurementState();
    std::cout << "measState.parameters" << std::endl;
    std::cout << "x: " << measState.parameters[0] << " " << measState.parameters[1] << " " << measState.parameters[2] << std::endl;
    std::cout << "measState.errors" << std::endl;
    dumpMatrix(measState.errors);

    TrackState updatedState = updateParameters(propState, measState,projMatrix36,projMatrix36T);
    std::cout << "updatedState" << std::endl;
    std::cout << "x: " << updatedState.parameters[0] << " " << updatedState.parameters[1] << " " << updatedState.parameters[2] << std::endl;
    std::cout << "p: " << updatedState.parameters[3] << " " << updatedState.parameters[4] << " " << updatedState.parameters[5] << std::endl;
    std::cout << "updatedState.errors" << std::endl;
    dumpMatrix(updatedState.errors);

    tmpInitState = updatedState;

  }

  return 0;

}
