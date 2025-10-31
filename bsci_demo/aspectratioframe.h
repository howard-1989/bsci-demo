#ifndef ASPECTRATIOFRAME_H
#define ASPECTRATIOFRAME_H

#include <QFrame>
#include <qcap.windef.h>

class AspectRatioFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AspectRatioFrame(QWidget *parent = nullptr);
    void setAspectRatio(double ratio);

    bool hasHeightForWidth() const override;
    int heightForWidth(int w) const override;

private:
    double m_aspectRatio;
};

#endif // ASPECTRATIOFRAME_H
