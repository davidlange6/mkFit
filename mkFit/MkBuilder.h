#ifndef MkBuilder_h
#define MkBuilder_h

#include <vector>

#include "HitStructures.h"


//------------------------------------------------------------------------------

#include "MkFitter.h"
#include "CandCloner.h"

#include <functional>
#include <mutex>

#include "Pool.h"

struct ExecutionContext
{
  Pool<CandCloner> m_cloners;
  Pool<MkFitter>   m_fitters;

  ExecutionContext()
  {
    m_cloners.populate();
    m_fitters.populate();
  }
};


//==============================================================================
// The usual
//==============================================================================

class Event;
class EventTmp;

class MkFitter;

class MkBuilder
{
private:
  void fit_one_seed_set(TrackVec& simtracks, int itrack, int end, MkFitter *mkfp);

  Event         *m_event;
  EventTmp      *m_event_tmp;
  EventOfHits    m_event_of_hits;

  std::vector<MkFitter*> m_mkfp_arr;

  int m_cnt=0, m_cnt1=0, m_cnt2=0, m_cnt_8=0, m_cnt1_8=0, m_cnt2_8=0, m_cnt_nomc=0;

public:
  typedef std::vector<std::pair<int,int>> CandIdx_t;

  MkBuilder();
  ~MkBuilder();

  // --------

  void begin_event(Event* ev, EventTmp* ev_tmp, const char* build_type);

  void fit_seeds();
  void fit_seeds_tbb();

  void end_event();

  // --------

  void quality_reset();
  void quality_process(Track& tkcand);
  void quality_print();

  // --------

  // Common foos for FindTracks() / FindTracksCloneEngine() ???

  void find_tracks_load_seeds(EventOfCandidates& event_of_cands); // for FindTracksBestHit
  void find_tracks_load_seeds();
  void find_tracks_in_layers(EtaBinOfCombCandidates &eb_of_cc, CandCloner &cloner, MkFitter *mkfp,
                             int start_seed, int end_seed, int ebin);

  // --------

  void FindTracksBestHit(EventOfCandidates& event_of_cands);
  void FindTracks();
  void FindTracksCloneEngine();
  void FindTracksCloneEngineTbb();
};

#endif
