#include <qbuffer.h>
#include "baghiralinkdrag.h"

static bool _accepted;

#define _TLO_ 0
#define _TO_ 4*sizeof(int)
#define _CLO_ 1*sizeof(int)
#define _CO_ 4*sizeof(int) + title.length()*sizeof(QChar)
#define _CO2_ 4*sizeof(int) + title->length()*sizeof(QChar)
#define _ILO_ 2*sizeof(int)
#define _IO_ 4*sizeof(int) + (title.length()+command.length())*sizeof(QChar)
#define _IO2_ 4*sizeof(int) + (title->length()+command->length())*sizeof(QChar)
#define _IxO_ 3*sizeof(int)

BaghiraLinkDrag::BaghiraLinkDrag(QString title, QString command, QString icon, int index, QWidget* dragSource) : QDragObject(dragSource, 0)

{
   _accepted = false;
   a.resize((title.length()+command.length()+icon.length())*sizeof(QChar)+4*sizeof(int));
   
   QChar* tmpChar;
   int tmpLength;
   
   tmpChar = const_cast<QChar*>(title.unicode());
   tmpLength = title.length();
   memcpy(a.data(), &tmpLength, sizeof(int));
   memcpy(a.data() + _TO_ , tmpChar, tmpLength*sizeof(QChar));
   
   tmpChar = const_cast<QChar*>(command.unicode());
   tmpLength = command.length();
   memcpy(a.data() + _CLO_ , &tmpLength, sizeof(int));
   memcpy(a.data() + _CO_, tmpChar, tmpLength*sizeof(QChar));
   
   tmpChar = const_cast<QChar*>(icon.unicode());
   tmpLength = icon.length();
   memcpy(a.data() + _ILO_, &tmpLength, sizeof(int));
   memcpy(a.data() + _IO_, tmpChar, tmpLength*sizeof(QChar));
   
   memcpy(a.data() + _IxO_, &index, sizeof(int));
}

BaghiraLinkDrag::~BaghiraLinkDrag()
{
}

bool BaghiraLinkDrag::decode( const QMimeSource * e, QString * title,  QString * command, QString * icon, int * index)
{
   QByteArray a = e->encodedData("application/baghiralink");
   
   if (a.size() < 4*sizeof(int)) // some empty stuff
   {
      return false;
   }
   
   QChar* tmpChar;
   int tmpLength;
   
   memcpy(&tmpLength, a.data(), sizeof(int));
   tmpChar = new QChar[tmpLength];
   memcpy(tmpChar, a.data() + _TO_, tmpLength*sizeof(QChar));
   title->setUnicode(tmpChar, tmpLength);
   delete tmpChar;
   
   memcpy(&tmpLength, a.data() + _CLO_, sizeof(int));
   tmpChar = new QChar[tmpLength];
   memcpy(tmpChar, a.data() + _CO2_, tmpLength*sizeof(QChar));
   command->setUnicode(tmpChar, tmpLength);
   delete tmpChar;
   
   memcpy(&tmpLength, a.data() + _ILO_, sizeof(int));
   tmpChar = new QChar[tmpLength];
   memcpy(tmpChar, a.data() + _IO2_, tmpLength*sizeof(QChar));
   icon->setUnicode(tmpChar, tmpLength);
   delete tmpChar; tmpChar = 0L;
   
   memcpy(index, a.data() + _IxO_, sizeof(int));
  
   return true;
}

bool BaghiraLinkDrag::accepted()
{
   return _accepted;
}

bool BaghiraLinkDrag::canDecode( const QMimeSource * e )
{
   return e->provides("application/baghiralink");
#if 0
   if (!e->provides("application/baghiralink"))
   {
      return false;
   }
   QByteArray a = e->encodedData("application/baghiralink");
   if (a.size() != BAGHIRALINK_BUFSIZE)
   {
      return false;
   }
   return true;
#endif
}

void BaghiraLinkDrag::setAccepted()
{
   _accepted = true;
}

const char * BaghiraLinkDrag::format ( int i ) const
{
   if (i == 0)
   {
      return "application/baghiralink";
   }
   return 0;
}

QByteArray BaghiraLinkDrag::encodedData ( const char * mimeType) const
{
   if (QString("application/baghiralink") == mimeType)
   {
      return a;
   }
   return QByteArray();
}
