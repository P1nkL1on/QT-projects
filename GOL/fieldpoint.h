#ifndef FIELDPOINT_H
#define FIELDPOINT_H

#include <QColor>
#include <QVector>
#include <QString>
#include <qdebug.h>

class FieldPoint
{
public:
    virtual QColor color() const = 0;
    virtual void step() = 0;
    virtual void think(const QVector<FieldPoint *> neight) = 0;
    virtual bool isDead() const = 0;
    virtual bool isEmpty() const = 0;
    virtual QString type() const = 0;
    virtual float health() const = 0;
    virtual void addHealth(const float X) = 0;
    virtual void setCrType(const QString &value) = 0;
};


class FieldMosh : public FieldPoint
{
private:
    float m_health = 0;
    FieldPoint *wantto = nullptr;
    int lastNeiCount = 0;
    QString crType = "Mosh";
public:
    FieldMosh() = default;
    FieldMosh(const float health):m_health (health){}
    void step() override{
        if (crType == "Mosh"){
            if (!isEmpty() && lastNeiCount >= 6 && m_health < 70)
                m_health ++;
            if (wantto == nullptr)
                return;
            wantto->addHealth(m_health / 2);
            m_health /= 2;
        }
        if (crType== "Biter"){
            if (!isEmpty())
                m_health /= 1.1;
            if (wantto == nullptr)
                return;

            wantto->addHealth(m_health);
            wantto->setCrType("Biter");
            crType = "Mosh";
            m_health = 0;
        }
    }

    void think(const QVector<FieldPoint *> neight) override{
        wantto = nullptr;
        if (crType == "Mosh"){
            QVector<FieldPoint *> newNei;
            for (int i = 0; i < neight.length(); ++i)
                if (neight[i]->type() == "Empty")
                    newNei << neight[i];
            lastNeiCount = neight.length() - newNei.length();   // count of nonempty neights
            if (newNei.length() <= 0)
                return;
            wantto = newNei[qrand() % newNei.length()];
        }
        if (crType == "Biter"){
            QVector<FieldPoint *> emptyNeis;
            QVector<FieldPoint *> emptyMosh;
            for (int i = 0; i < neight.length(); ++i){
                if (neight[i]->type() == "Empty")
                    emptyNeis << neight[i];
                if (neight[i]->type() == "FieldMosh")
                    emptyMosh << neight[i];
            }
            if (emptyMosh.length() <= 0)
            {
                if (emptyNeis.length() <= 0)
                    return;
                wantto = emptyNeis[qrand() % emptyNeis.length()];
                return;
            }
            wantto = emptyMosh[qrand() % emptyMosh.length()];
        }
    }

    QString type() const override{
        return (!isEmpty())? ("Field" + crType) : "Empty";
    }

    bool isDead() const override{
        return m_health <= 0.001;
    }

    bool isEmpty() const override{
        return isDead();
    }
    float health() const override;
    void addHealth(const float X) override{
        m_health += X;
    }
    QColor color() const override{
        if (crType == "Mosh")
            return (isEmpty())? Qt::white : QColor::fromRgb(0, (int)((m_health>=100)? 255 : 2.55 * m_health), 0);
        return (isEmpty())? Qt::yellow : QColor::fromRgb((int)((m_health>=100)? 255 : 2.55 * m_health), 0, 0);
    }
    void setCrType(const QString &value) override;
};

#endif // FIELDPOINT_H
