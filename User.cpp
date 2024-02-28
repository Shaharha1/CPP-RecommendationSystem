

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here
User::User(const std::string& username,const rank_map& rank,
std::shared_ptr<RecommendationSystem> rec_ptr) : _username(username) ,
_rank(rank), _rec_ptr(rec_ptr)
{
}

const std::string& User::get_name() const
{
  return _username;
}

void User::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features, double rate)
{
  sp_movie new_movie = (*_rec_ptr).add_movie(name, year, features);
  _rank[new_movie] = rate;
}

const rank_map& User::get_ranks() const
{
  return _rank;
}

sp_movie User::get_recommendation_by_content() const
{
  sp_movie re = (*_rec_ptr).recommend_by_content (*this);
  return re;
}

sp_movie User::get_recommendation_by_cf(int k) const
{
  sp_movie re = (*_rec_ptr).recommend_by_cf((*this),k);
  return re;
}

double User::get_prediction_score_for_movie(const std::string& name, int year,
                                       int k) const
{
  sp_movie movie = std::make_shared<Movie>(name,year);
  double re = (*_rec_ptr).predict_movie_score((*this), movie,k);
  return re;
}

std::ostream& operator<<(std::ostream& os,const User& user)
{
  os << "name: " << user.get_name() << std::endl;
  for(auto it= user.get_ranks().begin(); it != user.get_ranks().end(); it++)
  {
    os << (*it->first);
  }
  return os;
}