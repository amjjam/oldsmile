#include "../include/stringutils.H"

/*====================================================================
  std::vector<std::string> stringsep(std::string s, char c=' ') -
  separate a string into sub-strings that are separated by separation
  character c. Default separation character is ' '. Start by first
  skipping initial separation characters. 
  
  If first or last character is the separator a extra empty string
  will be generated for each. 
  ===================================================================*/
std::vector<std::string> stringsep(std::string s, char c){
  std::vector<std::string> o;
  unsigned int p;
  for(p=0;p<s.length();p++)
    if(s[p]!=c) break;

  if(p==s.length())
    return o;

  for(unsigned int i=p+1;i<s.length();i++)
    if(s[i]==c){
      o.push_back(s.substr(p,i-p));
      p=i+1;
    }
  if(s[s.length()-1]!=c)
    o.push_back(s.substr(p,s.length()-p));

  // Debugging
  // std::cout << "-----" << std::endl;
  // std::cout << o.size() << std::endl;
  // for(unsigned int i=0;i<o.size();i++)
  //   std::cout << o[i] << std::endl;
  // std::cout << "-----" << std::endl;

  return o;
}

/*====================================================================
  std::string stringcompress(std::string s, char c=' ') - replaces
  multipled consecutive occurrences of char with a single
  occurrence. Default character is space.
  ===================================================================*/
std::string stringcompress(std::string s, char c){
  std::string os;
  os.reserve(s.size());
  bool space=false;
  for(unsigned int i=0;i<s.length();i++){
    if(s[i]!=c){
      os+=s[i];
      space=false;
      //std::cout << i << std::endl;
    }
    else if(!space){
      os+=s[i];
      space=true;
      //std::cout << i << std::endl;
    }
  }
  return os;
}


/*====================================================================
  std::string stringtrim(std::string s, char c=' ') - remove leading
  and trailing characters from a string. Default character to remove
  is space.
  ===================================================================*/
std::string stringtrim(std::string s, char c){
  unsigned int iStart,iStop;
  for(iStart=0;iStart<s.length();iStart++)
    if(s[iStart]!=c) break;
  for(iStop=s.length()-1;iStop>=0;iStop--)
    if(s[iStop]!=c) break;
  
  return s.substr(iStart,iStop-iStart+1);
}

