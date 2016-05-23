#pragma once
#include <QPointer>
#include <iscore/tools/IdentifiedObjectAbstract.hpp>

namespace iscore
{
struct FocusManager : public QObject
{
        Q_OBJECT
    public:
        const IdentifiedObjectAbstract* get()
        { return m_obj; }

        template<typename T>
        void set(QPointer<const T> obj)
        {
            m_obj = obj.data();
            emit changed();
        }

        void set(nullptr_t)
        {
            m_obj.clear();
            emit changed();
        }

    signals:
        void changed();

    private:
        QPointer<const IdentifiedObjectAbstract> m_obj{};
};

struct FocusFacade
{
    private:
        FocusManager& m_mgr;

    public:
        FocusFacade(FocusManager& mgr):
            m_mgr{mgr}
        {

        }

        const IdentifiedObjectAbstract* get()
        { return m_mgr.get(); }
};
}