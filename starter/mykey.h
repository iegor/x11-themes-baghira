 
#ifndef MYKEY_H
#define MYKEY_H
#include <qstring.h>

class MyKey
{
public:
   MyKey(short key, short modFlags){modFlags_ = modFlags; key_ = key;}
   MyKey(){modFlags_ = 0; key_ = 0;}
   MyKey(QString & string)
   {
      QString tmpString = string.left(string.findRev('+'));
      modFlags_ = 0;
      if (tmpString.contains("256")) // QString.setNum(Qt::ShiftButton)
         modFlags_ |= Qt::ShiftButton;
      if (tmpString.contains("512")) // QString.setNum(Qt::ControlButton)
         modFlags_ |= Qt::ControlButton;
      if (tmpString.contains("1024")) // QString.setNum(Qt::AltButton)
         modFlags_ |= Qt::AltButton;
      tmpString = string.right(string.length() - string.findRev('+') - 1);
      key_ = tmpString.toShort();
   }
   short modFlags() const {return modFlags_;}
   short key()const {return key_;}
   bool operator==( const MyKey& myKey ) const
   {
      return (modFlags_ == myKey.modFlags() && key_ == myKey.key());
   }
   bool operator!=( const MyKey& myKey ) const
   {
      return (modFlags_ != myKey.modFlags() || key_ != myKey.key());
   }
   bool operator<( const MyKey& myKey ) const
   {
      return (!(modFlags_ < myKey.modFlags()) || key_ < myKey.key());
   }
   bool operator>( const MyKey& myKey ) const
   {
      return (!(modFlags_ > myKey.modFlags()) && key_ > myKey.key());
   }
   QString toString() const
   {
      QString string;
      QString numString;
      if (modFlags_ & Qt::ShiftButton)
      {
         numString.setNum(Qt::ShiftButton);
         string += numString;
         string += '+';
      }
      if (modFlags_ & Qt::ControlButton)
      {
         numString.setNum(Qt::ControlButton);
         string += numString;
         string += '+';
      }
      if (modFlags_ & Qt::AltButton)
      {
         numString.setNum(Qt::AltButton);
         string += numString;
         string += '+';
      }
      numString.setNum(key_);
      string += numString;
      return string;
   }
private:
   short modFlags_;
   short key_;
};

#endif
