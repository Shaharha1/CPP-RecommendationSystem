//
// Created by 24565 on 6/1/2022.
//

#include "RecommendationSystemLoader.h"
#include "UsersLoader.h"
#include <vector>
#include <cmath>

std::unique_ptr<RecommendationSystem> start()
{
  //Movie m("A",1999);
  //std::cout << m << std::endl;
  auto rs = RecommendationSystemLoader::create_rs_from_movies("RecommendationSystemLoader_input.txt");
  return rs;
}

int scalar_mult(const std::vector<double>& lhs, const
std::vector<double>& rhs)
{
  int sum = 0;
  for(int i=0; i<int(lhs.size()); i++)
  {
    sum += (lhs[i] * rhs[i]);
  }
  return sum;
}

double distance(std::vector<double>& vec)
{
  int sum = scalar_mult (vec,vec);
  return sqrt (sum);
}

int main(){
  //auto rs = start();
  //std::cout << *rs << std::endl;
  std::vector<int> m;
  m.push_back (7);
  m.push_back (2);
  m.push_back (9);
  m.push_back (1);
  double sum = 0;
  for(int i=0; i<int(m.size()); i++)
  {
    sum += (m[i] * m[i]);
  }
  sum = sqrt (sum);
  std::cout << sum;

  //auto users = UsersLoader::create_users("UsersLoader_input.txt",std::move
  //(rs));
  //sp_movie movie1 = users[0].get_recommendation_by_content();
  //sp_movie movie2 = users[0].get_recommendation_by_cf (1);
  //std::cout << (*movie2);
}