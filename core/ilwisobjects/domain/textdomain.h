#ifndef TEXTDOMAIN_H
#define TEXTDOMAIN_H

namespace Ilwis {
class KERNELSHARED_EXPORT TextDomain : public Domain
{
public:
    TextDomain();
    TextDomain(const Resource& res);

    virtual IlwisTypes valueType() const;
    virtual QString value(const QVariant& v) const;
    virtual Containement contains(const QVariant& ) const;
    IlwisTypes ilwisType() const;
private:
    QSharedPointer<Range> getRange() const;
};
typedef IlwisData<TextDomain> ITextDomain;
}

#endif // TEXTDOMAIN_H
