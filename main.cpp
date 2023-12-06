#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include<cassert>

using namespace std;

class Domain {
    // разработайте класс домена
   // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
public:
    explicit Domain(const string& name) {
        reverse_domain = { name.crbegin(), name.crend() };
        reverse_domain.push_back('.');
    }
    // разработайте operator==
    bool operator==(const Domain& other) const{
        return reverse_domain == other.reverse_domain;
    }

    bool operator<(const Domain& other) const{
      //  return reverse_domain < other.reverse_domain;
        return reverse_domain < other.reverse_domain;
    }
    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const{
        if (this->reverse_domain.size() > other.reverse_domain.size()) {
            return false;
        }
        string sub_domen = other.reverse_domain.substr(0, this->reverse_domain.size());
        return sub_domen == this->reverse_domain;
    }
    const string& Get() { return reverse_domain; }
private:
   string reverse_domain="";
};

template <typename InIterator>
class DomainChecker {
public:
   // template <typename InIterator>
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    explicit DomainChecker(InIterator begin, InIterator end)  : forbid_domains(begin, end) {
        sort(forbid_domains.begin(), forbid_domains.end());
        auto duble = unique(forbid_domains.begin(), forbid_domains.end(), [](Domain lhs, Domain rhs) {return lhs.IsSubdomain(rhs); });
        forbid_domains.erase(duble, forbid_domains.end());
    }
   
    
    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(Domain d) {
        
        auto upper=upper_bound(forbid_domains.begin(), forbid_domains.end(), d);
        if (upper == forbid_domains.begin()) {
            return false;
        }
        const auto upper1 = upper - 1;
        if ((upper1)->IsSubdomain(d)) {
            return true;
        }
        return false;
    }
    //Для тестирования
    vector<Domain> GetVector() { return forbid_domains; }
private:
    vector<Domain> forbid_domains;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}
template <typename Number>
const std::vector<Domain> ReadDomains(istream& input, Number n) {
    vector<Domain> domains;
    
    string line;
    for (Number i = 0; i < n; ++i) {
        getline(input, line);
        domains.push_back(Domain(line));
    }
    
    return domains;
}
void TestSubdomain() {
    {
        Domain forbid("gdz.ru");
        Domain d("biology.gdz.ru");
        assert(forbid.IsSubdomain(d));
    }
    {
        //при указании запрещенного домена, необходимо указывать путь до конца, 
        //иначе не работает, т.к. суффиксы не становятся префиксами
        Domain forbid("gdz");
        Domain d("gdz.math.ru");
        assert(!forbid.IsSubdomain(d));
    }
    {
        Domain forbid("ru");
        Domain d("programm.cpp.ru");
        assert(forbid.IsSubdomain(d));
    }
    {
        Domain forbid("cpp.ru");
        Domain d("programm.cpp.ua");
        assert(!forbid.IsSubdomain(d));
    }

}

void TestUniq() {
    string forbid_names[]={ "math.ru", "mmaps.me", "m.maps.me","maps.me", "com", "abc.com"};
    vector<Domain> forbids;
    for (auto name : forbid_names) {
        forbids.emplace_back(name);
    }
    DomainChecker checker(forbids.begin(), forbids.end());
    vector<Domain> correct_forbid=checker.GetVector();
    assert(correct_forbid.size() == 4);
    assert(find(correct_forbid.begin(), correct_forbid.end(), Domain("m.maps.me")) == correct_forbid.end());
    assert(find(correct_forbid.begin(), correct_forbid.end(), Domain("abc.com")) == correct_forbid.end());
}

void TestControlExample() {
    string forbid_names[] = { "gdz.ru", "maps.me", "m.maps.me","m.gdz.ru", "com" };
    vector<Domain> forbids;
    for (auto name : forbid_names) {
        forbids.emplace_back(name);
    }
    DomainChecker checker(forbids.begin(), forbids.end());
    vector<Domain> v = checker.GetVector();
    assert(checker.IsForbidden(Domain("gdz.ru")));
    assert(checker.IsForbidden(Domain("gdz.com")));
    assert(checker.IsForbidden(Domain("m.maps.me")));
    assert(checker.IsForbidden(Domain("alg.m.gdz.ru")));
    assert(checker.IsForbidden(Domain("maps.com")));
    assert(!checker.IsForbidden(Domain("maps.ru")));
    assert(!checker.IsForbidden(Domain("gdz.ua")));

}

void TestSimple() {
    vector<Domain> forbids;
    forbids.emplace_back("gdz.ru");
    DomainChecker checker(forbids.begin(), forbids.end());
    assert(checker.IsForbidden(Domain("history.gdz.ru")));
    assert(checker.IsForbidden(Domain("biology.gdz.ru")));
    assert(!checker.IsForbidden(Domain("freegdz.ru")));
}

int main() {
    TestSubdomain();
    TestUniq();
    TestSimple();
    TestControlExample();
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}