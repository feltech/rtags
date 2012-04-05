#ifndef INDEXER_H
#define INDEXER_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <AddMessage.h>
#include "Rdm.h"

typedef QHash<RTags::Location, Rdm::CursorInfo> SymbolHash;
typedef QHash<QByteArray, QSet<RTags::Location> > SymbolNameHash;
typedef QHash<Path, QSet<Path> > DependencyHash;
typedef QPair<QByteArray, quint64> WatchedPair;
typedef QHash<Path, QSet<WatchedPair> > WatchedHash;
typedef QHash<Path, QList<QByteArray> > InformationHash;

class IndexerImpl;
class Indexer : public QObject
{
    Q_OBJECT
public:

    Indexer(const QByteArray& path, QObject* parent = 0);
    ~Indexer();

    int index(const QByteArray& input, const QList<QByteArray>& arguments);

    static Indexer* instance();
    void setDefaultArgs(const QList<QByteArray> &args);
protected:
    void customEvent(QEvent* event);

signals:
    void indexingDone(int id);

private slots:
    void onJobDone(int id, const QByteArray& input);
    void onDirectoryChanged(const QString& path);

private:
    void initWatcher();
    IndexerImpl* mImpl;
    static Indexer* sInst;
};

#endif
