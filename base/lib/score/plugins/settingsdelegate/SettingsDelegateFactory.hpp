#pragma once
#include <score/plugins/customfactory/FactoryFamily.hpp>
#include <score_lib_base_export.h>

class QSettings;
namespace score
{
struct ApplicationContext;
class SettingsPresenter;
class SettingsDelegatePresenter;
class SettingsDelegateModel;
class SettingsDelegateView;

/**
 * @brief The SettingsDelegateFactory class
 *
 * Reimplement in order to provide custom user settings for the plug-in.
 */
class SCORE_LIB_BASE_EXPORT SettingsDelegateFactory
    : public score::Interface<SettingsDelegateFactory>
{
  SCORE_INTERFACE("f18653bc-7ca9-44aa-a08b-4188d086b46e")

public:
  virtual ~SettingsDelegateFactory();
  SettingsDelegatePresenter* makePresenter(
      score::SettingsDelegateModel& m,
      score::SettingsDelegateView& v,
      QObject* parent);

  virtual SettingsDelegateView* makeView() = 0;

  virtual std::unique_ptr<SettingsDelegateModel>
  makeModel(QSettings& settings, const score::ApplicationContext& ctx) = 0;

protected:
  virtual SettingsDelegatePresenter* makePresenter_impl(
      score::SettingsDelegateModel& m,
      score::SettingsDelegateView& v,
      QObject* parent)
      = 0;
};

using SettingsDelegateFactoryList = InterfaceList<score::SettingsDelegateFactory>;

template <typename Model_T, typename Presenter_T, typename View_T>
class SettingsDelegateFactory_T : public SettingsDelegateFactory
{
  std::unique_ptr<SettingsDelegateModel> makeModel(
      QSettings& settings, const score::ApplicationContext& ctx) override
  {
    return std::make_unique<Model_T>(settings, ctx);
  }

  score::SettingsDelegateView* makeView() override
  {
    return new View_T;
  }

  score::SettingsDelegatePresenter* makePresenter_impl(
      score::SettingsDelegateModel& m,
      score::SettingsDelegateView& v,
      QObject* parent) override
  {
    return new Presenter_T{safe_cast<Model_T&>(m), safe_cast<View_T&>(v),
                           parent};
  }
};

#define SCORE_DECLARE_SETTINGS_FACTORY(                                 \
    Factory, Model, Presenter, View, Uuid)                               \
  class Factory final                                                    \
      : public score::SettingsDelegateFactory_T<Model, Presenter, View> \
  {                                                                      \
    SCORE_CONCRETE(Uuid)                                        \
  };
}