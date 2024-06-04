#include "reassembler.hh"

using namespace std;


void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{

  auto itr_index = index_.begin();  
  auto itr_string = repo.begin();
  auto itr_flag = flag.begin();

  if (data.size() == 0 and !is_last_substring) {
    return;
  }

  if (data.size() == 0 and nextbytes_index_ == first_index) {
    output_.writer().close();
    nextbytes_index_ = 0;
    return;
  }

  if (nextbytes_index_ + output_.writer().available_capacity() - 1  < first_index || first_index + data.size() - 1 < nextbytes_index_) {
    return;
  }

  //处理data
  if (data.size() > output_.writer().available_capacity()) {
    
    if(first_index <= nextbytes_index_){
      if (data.size()+first_index -1  > nextbytes_index_ + output_.writer().available_capacity() -1 ) {
        data = data.substr(nextbytes_index_ - first_index, output_.writer().available_capacity()); //截取可用长度
        is_last_substring = false;
      }else {
        data = data.substr(nextbytes_index_ - first_index);   //截取到最后
      }
      first_index = nextbytes_index_;   // 更新截取之后的首字符索引

    }else {
        if (first_index + data.size() -1 > nextbytes_index_ + output_.writer().available_capacity()-1) {
          data = data.substr(0, nextbytes_index_+output_.writer().available_capacity() - first_index);
          is_last_substring = false;
        }
    }
  }else {
    if (first_index <= nextbytes_index_) {
      data = data.substr(nextbytes_index_ - first_index);
      first_index = nextbytes_index_;
    }else if (first_index + data.size()-1 > nextbytes_index_+output_.writer().available_capacity()-1) {
      data = data.substr(0,nextbytes_index_+output_.writer().available_capacity() - first_index);
      is_last_substring = false;
    }
  }


 while (!(index_.empty()) and itr_index != index_.end()) {
    if (*itr_index < first_index and 
        *itr_index + (*itr_string).size()<first_index+data.size() and 
        *itr_index + (*itr_string).size()-1 >= first_index ) {

      data = (*itr_string).substr(0, first_index - *itr_index) + data;
      first_index = *itr_index;
      buffered_size_ -= (*itr_string).size();

      itr_index = index_.erase(itr_index);
      itr_string = repo.erase(itr_string);
      itr_flag = flag.erase(itr_flag);

    }else if (*itr_index >= first_index and *itr_index + (*itr_string).size()<=first_index+data.size()) {
    //   index_.erase(itr_index);
      itr_index = index_.erase(itr_index);
      buffered_size_ -= (*itr_string).size();
      itr_string = repo.erase(itr_string);
      itr_flag = flag.erase(itr_flag);

    }else if (*itr_index >= first_index and *itr_index + (*itr_string).size()>first_index+data.size() and *itr_index <= first_index + data.size() ) {
      data = data + (*itr_string).substr(first_index+data.size() - *itr_index);
      buffered_size_ -= (*itr_string).size();
      itr_index = index_.erase(itr_index);
      itr_string = repo.erase(itr_string);
      is_last_substring = *itr_flag;
      itr_flag = flag.erase(itr_flag);
      
    }else if (*itr_index < first_index and *itr_index + (*itr_string).size() > first_index+data.size()) {
      return;
    }else {
      itr_index++;
      itr_flag++;
      itr_string++;
    }
    
 }

if (first_index == nextbytes_index_) {
  if (!(output_.writer().is_closed())) {
      output_.writer().push(data);
      nextbytes_index_ += data.size();
  }

  if (is_last_substring) {
    output_.writer().close();
    nextbytes_index_ = 0;
  }
}else {
  
  index_.push_back(first_index);
  repo.push_back(data);
  flag.push_back(is_last_substring);
  buffered_size_ += data.size();
}

}



uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return buffered_size_;
}

