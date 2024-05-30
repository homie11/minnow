#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return close_str;
}

void Writer::push( string data )
{
  // Your code here.

  auto size = min( available_capacity(), data.size() );
  if ( size == 0 )
    return;

  if ( size < data.size() )
    data = data.substr( 0, size );

  _queue.emplace_back( move( data ) );
  buffer_view.emplace_back( _queue.back() );
  total_push += size;
  input_size += size;
  available_capacity_ = capacity_ - input_size;
  return;
}

void Writer::close()
{
  // Your code here.
  close_str = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - input_size;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return total_push;
}

bool Reader::is_finished() const
{
  bool fullypop = ( input_size == 0 );

  return close_str && fullypop;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return total_pop;
}

string_view Reader::peek() const
{
  // Your code here.
  if ( buffer_view.empty() )
    return string_view {};
  return buffer_view.front();
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if ( buffer_view.empty() )
    return;

  auto pop_size = min( input_size, len );
  total_pop += pop_size;
  input_size -= pop_size;
  available_capacity_ = capacity_ - input_size;

  while ( pop_size > 0 ) {
    auto next_size = buffer_view.front().size();
    if ( pop_size >= next_size ) {
      buffer_view.pop_front();
      _queue.pop_front();
    } else {
      buffer_view.front().remove_prefix( pop_size );
      break;
    }
    pop_size -= next_size;
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return input_size;
}

// byte_stream.cc

