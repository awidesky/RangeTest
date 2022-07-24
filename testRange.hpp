#ifndef TESTRANGE_HPP
#define TESTRANGE_HPP

#include <future>
#include <thread>
#include <vector>
#include <utility>
#include <iostream>
#include <chrono>

template <typename R, typename Func, typename Comparator >
std::vector<R> testRange(int from, int end, Func f, Comparator comp, std::ostream& out = std::cout, int pool=std::thread::hardware_concurrency()) {
  out << "1thread number : " << std::thread::hardware_concurrency() << "\n";
  out << "\ntesting from " << std::hex << from << " to " << std::hex << end << std::dec << "\t(" << (end - from) << ")" << "\n";
  int cnt = (end - from) / pool;
  int to = cnt + from;
  
  auto start = std::chrono::steady_clock::now();
  std::vector<std::future<std::vector<R>>> v;
  for(int i = 0; i < pool; i++) {
    v.push_back(std::async(std::launch::async, [from, to, &f, &comp, &out](){
      std::vector<R> list;
      list.push_back(f(from));
      for(int j = from + 1; j < to; j++) {
      R var = f(j);
      if (comp(list[0], var)) {
            list.clear();
            list.push_back(var);
      } else if (!comp(var, list[0])) { // two values are same
            list.push_back(var);
      }
      if(!(j % 500)) out << "\n500 iteration done!";
      }
      return list;
    }));
    
    from = to;
    
    if (i == pool - 1) {
      to = end + 1;
    } else {
      to += cnt;
    }
  }
  out << "all task(" << v.size() << ") queued!\n";
  std::vector<R> re = v[0].get();
  for(int j = 1; j < pool; j++) {
    out << "\n#" << j - 1 << " task executed\n";
    std::vector<R> re2 = v[j].get();
    if (comp(re[0], re2[0])) {
          re.clear();
          re.insert(std::end(re), std::begin(re2), std::end(re2));
    } else if (!comp(re2[0], re[0])) { // two values are same
          re.insert(std::end(re), std::begin(re2), std::end(re2));
    }
  }
  auto endTime = std::chrono::steady_clock::now();
  auto diff = endTime - start;
  out << "\n" << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
  return re;
}

#endif