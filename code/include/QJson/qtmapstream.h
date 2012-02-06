/*
    @author Ben Lau

    License : New BSD

 */

#ifndef QTMAPSTREAM_H
#define QTMAPSTREAM_H

#include <QtCore>
#include <QVariant>
#include <QMap>

/// The QtMapStream provides serialization of object class to QVariantMap.
/**
 QVariantMap is a synonym of QMap<QString, QVariant>. It could represent any complex objects. Convert
 a QVariantMap instance to data serialization formats like XML , JSON and YAML is much easier than
 a complex objects in C++. However, convert a C++ class to QVariantMap is not strict forward. User
 have to implements their own convertion function.

 The QtMapStream provides an interface and utility to convert user's own C++ class , which is not based
 on QObject , to QVariantMap and vice versa.

 Example class:

 \code
 class Book {
 public:
    QString title;
    int pages;
    qreal rating;
 }
 \endcode

 You should add the QT_MAP_STREAMABLE / QT_MAP_STREAMABLE_ITEM macro to the class declaration:

 \code
 class Book {
 public:
    QString title;
    int pages;
    qreal rating;

    QT_MAP_STREAMABLE(
        QT_MAP_STREAMABLE_ITEM(QString,title)
        QT_MAP_STREAMABLE_ITEM(int,pages)
        QT_MAP_STREAMABLE_ITEM(qreal,rating)
    )
 };
 \endcode

 The macro will add <<,>> operators overloading with taking a QtMapStream to the class.

 Convert to QVariantMap

 \code

 Book book;
 book.title = "Qt Developer Guides";
 book.pages = 180;
 book.rating = 10.0;

 QtMapStream stream;
 book >> stream;

 QVariantMap source = stream.source(); // It will return the QVariantMap writen by the book instance.

 qDebug() << source; // QMap(("pages", QVariant(int, 180) ) ( "rating" ,  QVariant(double, 10) ) ( "title" ,  QVariant(QString, "Qt Developer Guides") ) )

 \endcode


 Convert from QVariantMap

 \code

 Book book;
 QVariantMap map;
 map["title"] = "Qt Developer Guides";
 map["pages"] = 180;
 map["rating"] = 10.0;

 QtMapStream stream(map);
 book << stream; // The book instance will be filled with the information in QVariantmap

 QVariantMap remaining = stream.source(); // After run the << stream operators, written field will be removed from the source. Call source() should return an empty map.
 \endcode


 QtMapStream also support QList type. However, QList type can not be converted to QVariant directly. You should declare the QList type with Q_DECLARE_METATYPE first.

 Example:

 \code

 Q_DECLARE_METATYPE(QList<QString>)

 class BookV2 {
 public:
    QString title;
    int pages;
    qreal rating;
    QList<QString> authors;

    QT_MAP_STREAMABLE(
        QT_MAP_STREAMABLE_ITEM(QString,title)
        QT_MAP_STREAMABLE_ITEM(int,pages)
        QT_MAP_STREAMABLE_ITEM(qreal,rating)
        QT_MAP_STREAMABLE_ITEM(QList<QString> ,authors)
    )
 };
 \endcode


 License: New BSD
 */

class Q_DECL_EXPORT QtMapStream
{   
public:

    /// The abstract type of QtMapStream::Item
    /** It is an internal class. Instead of using it directly, you
      should use QT_MAP_STREAMABLE / QT_MAP_STREAMABLE_ITEM macro
      to create the code to use this class.
     */
    class AbstractItem {
    public:
        virtual QString key() = 0 ;
        virtual void setValue(QVariant value) = 0 ;
        virtual QVariant value() = 0;
    };

    /// A unit of item used in streaming
    /** It is an internal class. Instead of using it directly, you
      should use QT_MAP_STREAMABLE / QT_MAP_STREAMABLE_ITEM macro
      to create the code to use this class.
     */
    template <typename T>
    class Item : public AbstractItem {
    public:
        Item(QString key, T* value) : m_key(key),m_value(value){}

        QString key() {
            return m_key;
        }

        void setValue(QVariant value) {
            *m_value = value.value<T>();
        }

        QVariant value() {
            QVariant v;
            v.setValue<T>(*m_value);
            return v;
        }

    private:
        QString m_key;
        T* m_value;
    };

    /// Construct a QtMapSerializer instance
    QtMapStream();

    /// Construct a QtMapSerializer instance and set the read/write source to provided argument
    QtMapStream(QVariantMap source);

    /// Set the source for read/write
    void setSource(QVariantMap source);

    /// Get the source of read/write
    QVariantMap source();

    QtMapStream& addMap(const QString& name,const QMap<QString, QString>& map);
    QtMapStream& addMap(const QString& name,const QMap<QString, QVariant>& map);

    QVariant getValue(const QString& name){return m_source[name];};
    /// Read from QtMapStream::Item type
    /** It is an internal function. Instead of using it directly, you
      should use QT_MAP_STREAMABLE / QT_MAP_STREAMABLE_ITEM macro
      to create the code to use this function.
     */
    QtMapStream& operator<<(QtMapStream::AbstractItem *item);

    /// Write to QtMapStream::Item type
    /** It is an internal function. Instead of using it directly, you
      should use QT_MAP_STREAMABLE / QT_MAP_STREAMABLE_ITEM macro
      to create the code to use this function.
     */
    QtMapStream& operator>>(QtMapStream::AbstractItem *item);

private:
    QVariantMap m_source;

};

/// The QT_MAP_STREAMABLE_ITEM is used within QT_MAP_STREAMABLE macro
/**
  @see QT_MAP_STREAMABLE
 */
#define QT_MAP_STREAMABLE_ITEM(Type,Member) \
    QtMapStream::Item< Type > map_stream_field_ ## Member ( #Member , &Member); \
    items.append(&map_stream_field_ ## Member);

#define _QT_MAP_STREAMABLE_BEGIN() \
    QList<QtMapStream::AbstractItem *> items;

#define _QT_MAP_STREAMABLE_MIDDLE(OPS) \
    foreach (QtMapStream::AbstractItem * item,items) { \
        stream OPS item; \
    }

#define _QT_MAP_STREAMABLE_END()

/// The QT_MAP_STREAMABLE macro should be pushed in the class declaration to declare to support map streaming
#define QT_MAP_STREAMABLE(ITEMS...) \
   public:\
    inline void operator<<(QtMapStream& stream){ \
        _QT_MAP_STREAMABLE_BEGIN() \
        ITEMS \
        _QT_MAP_STREAMABLE_MIDDLE(>>) \
        _QT_MAP_STREAMABLE_END()\
    } \
    inline void operator>>(QtMapStream& stream){ \
        _QT_MAP_STREAMABLE_BEGIN() \
        ITEMS \
        _QT_MAP_STREAMABLE_MIDDLE(<<) \
        _QT_MAP_STREAMABLE_END()\
    }

/// The QT_MAP_STREAMABLE2 macro is used when your class inherited "to" other.
#define QT_MAP_STREAMABLE2(PARENT,ITEMS...) \
   public:\
    inline void operator<<(QtMapStream& stream){ \
        _QT_MAP_STREAMABLE_BEGIN() \
        ITEMS \
        _QT_MAP_STREAMABLE_MIDDLE(>>) \
        *(PARENT*)this << stream; \
        _QT_MAP_STREAMABLE_END()\
    } \
    inline void operator>>(QtMapStream& stream){ \
        _QT_MAP_STREAMABLE_BEGIN() \
        ITEMS \
        _QT_MAP_STREAMABLE_MIDDLE(<<) \
        *(PARENT*)this >> stream; \
        _QT_MAP_STREAMABLE_END()\
    }


#endif // QTMAPSTREAM_H
