#include<iostream>
class a{
public:
	a():n(0){
		std::cout << "A-constructor" << std::endl;
	}
	a(const int & n_):n(n_){
		std:: cout << "A-NewConstrutor" << std::endl;
	}
	int n;
};
class b :virtual public a{
	public:
	b(){
		this->n = 10;
		std::cout << "B-constructor"<< std::endl;
	}
	b(const int& n2_):a(n2_),n2(n2_){
		std::cout << "B-NewConstructor" << std::endl;
	}
	int n2;
};
class d : virtual public a{
	public: int n3;
	d(){
		this->n = 20;	
		std::cout << "D-constructor" << std::endl;
	}
	d(const int &n3_):a(n3_),n3(n3_){
		std::cout << "D-NewConstructor" << std::endl;
	}
};
class c: public b,public d{
	public:
	 c():n3(0){std::cout<<"C-constructor"<<this->n<<std::endl;}
	 c(const int& n3_):n3(n3_){
		 std::cout<<"C-NewConstructor"<< std::endl;
	 }
	int n3;
};
int main(){
	c test(1);
	//std::cout << test.n2 << std::endl;
	return 0;
}