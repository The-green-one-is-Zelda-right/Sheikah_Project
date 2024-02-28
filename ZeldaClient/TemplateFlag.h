#pragma once

template <typename Enum, typename T>
class TemplateFlag
{
private:
	T flags;

public:
	TemplateFlag& operator=(const TemplateFlag<Enum, T>&) = default;
	TemplateFlag&& operator=(TemplateFlag<Enum, T>&&) noexcept = default;

public:
	bool operator==(const TemplateFlag<Enum, T>& _flags) const
	{
		return this->flags == _flags.flags;
	}

	bool operator!=(const TemplateFlag<Enum, T>& _flags) const
	{
		return this->flags != _flags.flags;
	}

    TemplateFlag operator|(Enum _enum) const
    {
        TemplateFlag temp(*this);
        temp.flags |= static_cast<T>(_enum);
        return temp;
    }

    TemplateFlag& operator|=(Enum _enum)
    {
        flags |= static_cast<T>(_enum);
        return *this;
    }

    TemplateFlag operator&(Enum _enum) const
    {
        TemplateFlag temp(*this);
        temp.flags &= ~static_cast<T>(_enum);
        return temp;
    }

    TemplateFlag& operator&=(Enum _enum)
    {
        flags &= ~static_cast<T>(_enum);
        return *this;
    }

    TemplateFlag operator^(Enum _enum) const
    {
        TemplateFlag temp(*this);
        temp.flags ^= static_cast<T>(_enum);
        return temp;
    }

    TemplateFlag& operator^=(Enum _enum)
    {
        flags ^= static_cast<T>(_enum);
        return *this;
    }

    TemplateFlag operator~() const
    {
        TemplateFlag result;
        result.flags = ~flags;
        return result;
    }

    operator T() const
    {
        return flags;
    }
};

