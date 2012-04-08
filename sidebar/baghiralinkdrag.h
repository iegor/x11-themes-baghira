#ifndef _BAGHIRALINKDRAG_H_
#define _BAGHIRALINKDRAG_H_

#include <qdragobject.h>
#include <qstring.h>

class BaghiraLinkDrag : public QDragObject
{
//    Q_OBJECT
      
public:
   BaghiraLinkDrag(QString title, QString command, QString icon, int index = -1, QWidget* dragSource = 0);
   ~BaghiraLinkDrag();
   static bool decode( const QMimeSource * e, QString * title,  QString * command, QString * icon, int * index);
   static bool canDecode( const QMimeSource * e);
   static void setAccepted( );
   virtual const char * format ( int i = 0 ) const;
   virtual QByteArray encodedData ( const char * ) const;
   static bool accepted( );
private:
   QByteArray a;
};

#endif
