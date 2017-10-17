#pragma once
#include <Midi/MidiProcessMetadata.hpp>
#include <Process/Process.hpp>

#include <Midi/MidiNote.hpp>
#include <score/tools/Clamp.hpp>

namespace Midi
{

class SCORE_PLUGIN_MIDI_EXPORT ProcessModel final
    : public Process::ProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(Midi::ProcessModel)
  Q_OBJECT

public:
  explicit ProcessModel(
      const TimeVal& duration,
      const Id<Process::ProcessModel>& id,
      QObject* parent);

  explicit ProcessModel(
      const ProcessModel& source,
      const Id<Process::ProcessModel>& id,
      QObject* parent);

  template <typename Impl>
  explicit ProcessModel(Impl& vis, QObject* parent)
      : Process::ProcessModel{vis, parent}
  {
    vis.writeTo(*this);
  }

  ~ProcessModel() override;

  score::EntityMap<Note> notes;

  void setDevice(const QString& dev);
  const QString& device() const;

  void setChannel(int n);
  int channel() const;

  std::vector<Process::Port*> inlets() const override;
  std::vector<Process::Port*> outlets() const override;

  std::unique_ptr<Process::Port> outlet;
signals:
  void notesChanged();
  void deviceChanged(const QString&);
  void channelChanged(int);

private:
  void setDurationAndScale(const TimeVal& newDuration) override;
  void setDurationAndGrow(const TimeVal& newDuration) override;
  void setDurationAndShrink(const TimeVal& newDuration) override;

  QString m_device;
  int m_channel{1};
};

}