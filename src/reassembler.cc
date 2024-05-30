#include "reassembler.hh"

using namespace std;


void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.
  // (void)first_index;
  // (void)data;
  // (void)is_last_substring;
  auto itr_index = index_.begin();  
  auto itr_string = repo.begin();
  auto itr_flag = flag.begin();

  if (data.size() > output_.writer().available_capacity()) {
    
    if(first_index < nextbytes_index_){
      if (data.size()+first_index > nextbytes_index_) {
        data = data.substr(nextbytes_index_ - first_index, output_.writer().available_capacity());
        first_index = nextbytes_index_;
      }
    }else if (first_index == nextbytes_index_) {
      data = data.substr(0,output_.writer().available_capacity());
    }else {
      return;
    }
  }

  

  while (!(index_.empty()) && itr_index !=index_.end() && *itr_index != first_index) {  //search in reassembler
      itr_index++;
      itr_flag++;
      itr_string++;
  }

  if (first_index == nextbytes_index_) {    //whether push in stream?

    if(!(output_.writer().is_closed())){
          output_.writer().push(data);
    }
    if (is_last_substring) {
      output_.writer().close();
      nextbytes_index_ = 0;
    }
    nextbytes_index_ += data.size();
    
    if(itr_index != index_.end()){  // founded in!!
      buffered_size_ -= (*itr_string).size();
      index_.erase(itr_index);
      repo.erase(itr_string);
      flag.erase(itr_flag);
    }

  }else {
    if (itr_index == index_.end()) {
      index_.push_back(first_index);
      repo.push_back(data);
      flag.push_back(is_last_substring);
      buffered_size_ += data.size();
    }else {
      return;
    }

  }

}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return buffered_size_;
}

