// Generated by gencpp from file serial/trama.msg
// DO NOT EDIT!


#ifndef SERIAL_MESSAGE_TRAMA_H
#define SERIAL_MESSAGE_TRAMA_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/String.h>

namespace serial
{
template <class ContainerAllocator>
struct trama_
{
  typedef trama_<ContainerAllocator> Type;

  trama_()
    : trama()  {
    }
  trama_(const ContainerAllocator& _alloc)
    : trama(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::String_<ContainerAllocator>  _trama_type;
  _trama_type trama;





  typedef boost::shared_ptr< ::serial::trama_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::serial::trama_<ContainerAllocator> const> ConstPtr;

}; // struct trama_

typedef ::serial::trama_<std::allocator<void> > trama;

typedef boost::shared_ptr< ::serial::trama > tramaPtr;
typedef boost::shared_ptr< ::serial::trama const> tramaConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::serial::trama_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::serial::trama_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::serial::trama_<ContainerAllocator1> & lhs, const ::serial::trama_<ContainerAllocator2> & rhs)
{
  return lhs.trama == rhs.trama;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::serial::trama_<ContainerAllocator1> & lhs, const ::serial::trama_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace serial

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::serial::trama_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::serial::trama_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::serial::trama_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::serial::trama_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::serial::trama_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::serial::trama_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::serial::trama_<ContainerAllocator> >
{
  static const char* value()
  {
    return "91d4cca73e4fe47672e54da2c75435ef";
  }

  static const char* value(const ::serial::trama_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x91d4cca73e4fe476ULL;
  static const uint64_t static_value2 = 0x72e54da2c75435efULL;
};

template<class ContainerAllocator>
struct DataType< ::serial::trama_<ContainerAllocator> >
{
  static const char* value()
  {
    return "serial/trama";
  }

  static const char* value(const ::serial::trama_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::serial::trama_<ContainerAllocator> >
{
  static const char* value()
  {
    return "std_msgs/String trama\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/String\n"
"string data\n"
;
  }

  static const char* value(const ::serial::trama_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::serial::trama_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.trama);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct trama_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::serial::trama_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::serial::trama_<ContainerAllocator>& v)
  {
    s << indent << "trama: ";
    s << std::endl;
    Printer< ::std_msgs::String_<ContainerAllocator> >::stream(s, indent + "  ", v.trama);
  }
};

} // namespace message_operations
} // namespace ros

#endif // SERIAL_MESSAGE_TRAMA_H
