#pragma once

#include <memory>
#include <functional>

#include <giomm.h>

/* Forward declarations */
class IceAgent;
enum class IceAgentState;

class PeerRelay
{
public:
  PeerRelay(Glib::RefPtr<Glib::MainLoop> mainloop,
            int gamePort,
            int peerId,
            std::string const& peerLogin,
            std::string const& stunIp,
            std::string const& turnIp,
            std::string const& turnUser,
            std::string const& turnPassword);
  virtual ~PeerRelay();

  typedef std::function<void (PeerRelay*, std::string const&)> CandidateGatheringDoneCallback;
  void gatherCandidates(CandidateGatheringDoneCallback cb);

  typedef std::function<void (PeerRelay*, IceAgentState const&)> IceAgentStateCallback;
  void setIceAgentStateCallback(IceAgentStateCallback cb);

  int localGameUdpPort() const;

  std::shared_ptr<IceAgent> iceAgent() const;

  int port() const;

  std::string const& peerLogin() const;

protected:
  bool onGameReceive(Glib::IOCondition condition);
  Glib::RefPtr<Glib::MainLoop> mMainloop;
  Glib::RefPtr<Gio::Socket> mLocalSocket;
  int mPeerId;
  std::string mPeerLogin;
  int mLocalGameUdpPort;
  std::shared_ptr<IceAgent> mIceAgent;
  Glib::RefPtr<Gio::SocketAddress> mGameAddress;
  IceAgentStateCallback mIceAgentStateCallback;
};
