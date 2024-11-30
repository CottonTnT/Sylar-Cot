#ifndef __LOGFORMATITEM_H__
#define __LOGFORMATITEM_H__

#include <memory>

namespace Cot {

class LogEvent;

class FormatItemBase {
public:
    using Sptr       = std::shared_ptr<FormatItemBase>;
    FormatItemBase() = default;

    virtual ~FormatItemBase() = default;
    virtual auto format(std::ostream& os, std::shared_ptr<LogEvent> event)
        -> void = 0;
    virtual auto clone()
        -> FormatItemBase::Sptr = 0;

private:
};

template <typename Item>
class FormatItemImpl : FormatItemBase {
public:
    template <typename... Ts>
    FormatItemImpl(Ts... ts)
        : item(std::forward<Ts>(ts)...) {};

    auto format(std::ostream& os, std::shared_ptr<LogEvent> event)
        -> void override
    {
        item.format(os, event);
    }

    auto clone()
        -> FormatItemBase::Sptr override
    {
        return std::make_shared<FormatItemBase>();
    }
    ~FormatItemImpl() = default;

private:
    Item item;
};

class FormatItem {
public:
    template <typename Item>
    FormatItem(Item item)
        : item_ptr(std::make_unique<Item>(std::forward<Item>(item)))
    {
    }

    auto format(std::ostream& os, std::shared_ptr<LogEvent> event)
        -> void;

    auto clone() -> FormatItemBase::Sptr;
    ~FormatItem() = default;

private:
    std::unique_ptr<FormatItemBase> item_ptr;
};

class FormatItemFactoryBase {
public:
    virtual auto create()
        -> FormatItemBase::Sptr = 0;
    virtual ~FormatItemFactoryBase();

private:
};

template <typename Item>
class FormatItemFactoryImpl : FormatItemFactoryBase {
public:
    auto create()
        -> FormatItemBase::Sptr override
    {
        return std::make_shared<FormatItemBase>();
    }

private:
};

} //namespace  Cot
#endif