#include "Data.hpp"

// Gradient descent optimization method
std::vector<double> GradientDescent(const Data & data);

// Adam optimization method
std::vector<double> Adam(const Data & data);

// Heavy ball optimization method
std::vector<double> HeavyBall(const Data & data);


// Main function
int main(){

    // Load input parameters from JSON file
    Data data("data.json");

    std::function<std::vector<double>(const Data &)> Method;
    
    // Choose optimization method based on input
    if(data.method==0){
        Method = GradientDescent;
        std::cout<<"Using gradient descent."<<std::endl;
    } else if(data.method==1){
        Method = Adam;
        std::cout<<"Using Adam approximation."<<std::endl;
    } else if(data.method==2){
        Method = HeavyBall;
        std::cout<<"Using heavy ball approximation."<<std::endl;
    } else {
        std::cerr << "\n\nInvalid choice of learningMetod, insert a value between 0 and 2.\n\n" << std::endl;
    }
    std::cout << "----------------------------------"<< std::endl;

    // Perform optimization
    std::vector<double> final_x = Method(data);

    // Output final result
    std::cout << "[" << final_x[0] << "," << final_x[1] << "]"<< std::endl;
    std::cout << "Value in this point is f(min_x)=" << data.fun(final_x) << std::endl;
    std::cout << "----------------------------------"<< std::endl;

    return 0;
}

// Gradient descent optimization method
std::vector<double> GradientDescent(const Data & data){

    std::vector<double> xk1(data.x0);
    bool converged=false;

    for (size_t it = 1; it <= data.max_it && !converged; ++it){
        std::vector<double> xk(xk1);
        double a=data.Alpha(it,xk);

        xk1=xk-(a*data.Grad(xk));
        
        if(norm(xk-xk1)< data.tol_x or norm(data.Grad(xk)) < data.tol_df){
            converged=true;
            std::cout << "Method converged in " << it << " iterations to: ";
        }
    }
    if(!converged){
        std::cout << "Not converged in " << data.max_it << " iterations, final approximation: ";
    }

    return xk1;
}

// Adam optimization method
std::vector<double> Adam(const Data & data){

    std::vector<double> m(data.x0.size());
    std::vector<double> v(m);

    std::vector<double> xk1(data.x0);

    bool converged = false;


    for (size_t t = 1; t <= data.max_it && !converged; ++t){
        std::vector<double> xk(xk1);

        std::vector<double> gradient=data.Grad(xk);
        for (size_t i = 0; i < xk.size(); ++i) {
            m[i]=data.gamma1*m[i]+(1-data.gamma1)*gradient[i];
            v[i]=data.gamma2*v[i]+(1-data.gamma2)*pow(gradient[i],2);
            double mHat=m[i]/(1- pow(data.gamma1,t));
            double vHat=v[i]/(1- pow(data.gamma2,t));
            xk1[i] = xk[i] - data.Alpha(t,xk)*mHat/(sqrt(vHat)+data.epsilon);
        }

        if(norm(xk-xk1)<data.tol_x or norm(data.Grad(xk)) < data.tol_df){
            converged=true;
            std::cout << "Method converged in " << t << " iterations to: ";
        }
    }
    if(!converged){
        std::cout << "Not converged in " << data.max_it << " iterations, final approximation: ";
    }
    return xk1;

}

// Heavy ball optimization method
std::vector<double> HeavyBall(const Data & data){
    std::vector<double> xk1(data.x0-data.Alpha(0,data.x0)*data.Grad(data.x0));

    bool converged = false;

    for (size_t t = 1; t <= data.max_it && !converged; ++t) {
        std::vector<double> xk(xk1);

        xk1=xk - data.Alpha(t,xk)*data.Grad(xk)+data.eta*(xk-xk1);

        if(norm(xk-xk1)<data.tol_x or norm(data.Grad(xk)) < data.tol_df){
            converged=true;
            std::cout << "Method converged in " << t << " iterations to: ";
        }
    }
    if(!converged){
        std::cout << "Not converged in " << data.max_it << " iterations, final approximation: ";
    }
    return xk1;
}
