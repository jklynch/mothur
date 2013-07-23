//
//  svm.hpp
//  support vector machine
//
//  Created by Joshua Lynch on 6/19/2013.
//  Copyright (c) 2013 Schloss Lab. All rights reserved.
//

#ifndef svm_hpp_
#define svm_hpp_

#include <algorithm>
#include <cmath>
#include <deque>
#include <exception>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

// a FeatureVector is an Observation ....
// For the purpose of training a support vector machine
// we need to calculate a dot product between two feature
// vectors.  In general these feature vectors are not
// restricted to lists of doubles, but in this implementation
// feature vectors will be vectors of doubles.
typedef std::vector<double> FeatureVector;
typedef std::vector<double> Observation;

// A dataset is a collection of labeled observations (or labeled
// feature vectors).  The ObservationVector typedef is a vector
// of pointers to ObservationVectors.  Pointers are used here since
// datasets will be rearranged many times during cross validation.
// Using pointers to Observations makes copying the elements of
// an ObservationVector cheap.
typedef std::vector<Observation*> ObservationVector;

// Training a support vector machine requires labeled data.  The
// Label typedef defines what will constitute a class 'label' in
// this implementation.
typedef std::string Label;
typedef std::vector<Label> LabelVector;
typedef std::set<Label> LabelSet;

// Pairs of class labels are important because a support vector machine
// can only learn two classes of data.  The LabelPair typedef is a vector
// even though a std::pair might seem more natural, but it is useful to
// iterate over the pair.
typedef std::vector<Label> LabelPair;
LabelPair make_label_pair(const Label& one, const Label& two);

// Learning to classify a dataset with more than two classes requires
// training a separate support vector machine for each pair of classes.
// The LabelPairSet typedef defines a container for the collection of
// all unique label pairs for a set of data.
typedef std::set<LabelPair> LabelPairSet;

// A dataset is a set of observations with associated labels.  The
// LabeledObservation typedef is a label-observation pair intended to
// hold one observation and its corresponding label.
typedef std::pair<Label, Observation*> LabeledObservation;

// A LabeledObservationVector is a container for an entire dataset (or a
// subset of an entire dataset).
typedef std::vector<LabeledObservation> LabeledObservationVector;

// Dividing a dataset into training and testing sets while maintaing equal
// representation of all classes is done using a LabelToLabeledObservationVector.
// This container is used to divide datasets into groups of LabeledObservations
// having the same label.
typedef std::map<Label, LabeledObservationVector> LabelToLabeledObservationVector;

// A support vector machine uses +1 and -1 in calculations to represent
// the two classes of data it is trained to distinguish.  The NumericClassToLabel
// container is used to record the labels associated with these integers.
typedef std::map<int, Label> NumericClassToLabel;


typedef double Parameter;
typedef std::string ParameterName;
typedef std::vector<double> ParameterRange;
typedef std::map<ParameterName, ParameterRange> ParameterRangeMap;

typedef std::map<ParameterName, Parameter> ParameterMap;
typedef std::vector<ParameterMap> ParameterMapVector;
typedef std::stack<Parameter> ParameterStack;

class ParameterSetBuilder {
public:
    ParameterSetBuilder(ParameterRangeMap parameterRangeMap) {
        std::stack<std::pair<ParameterName, ParameterStack> > stackOfParameterRanges;
        std::stack<std::pair<ParameterName, ParameterStack> > stackOfEmptyParameterRanges;
        ParameterMap nextParameterSet;
        int parameterSetCount = 1;
        for ( ParameterRangeMap::iterator i = parameterRangeMap.begin(); i != parameterRangeMap.end(); i++ ) {
            parameterSetCount *= i->second.size();
            ParameterName parameterName = i->first;
            ParameterStack emptyParameterStack;
            stackOfEmptyParameterRanges.push(make_pair(parameterName, emptyParameterStack));
        }
        // get started
        for ( int n = 0; n < parameterSetCount; n++ ) {
            std::cout << "n = " << n << std::endl;
            // pull empty stacks off until there are no empty stacks
            while ( stackOfParameterRanges.size() > 0 and stackOfParameterRanges.top().second.size() == 0 ) {
                std::cout << "  empty parameter range: " << stackOfParameterRanges.top().first << std::endl;
                stackOfEmptyParameterRanges.push(stackOfParameterRanges.top());
                stackOfParameterRanges.pop();
            }

            // move to the next value for the parameter at the top of the stackOfParameterRanges
            if ( stackOfParameterRanges.size() > 0 ) {
                std::cout << "  moving to next value for parameter " << stackOfParameterRanges.top().first << std::endl;
                std::cout << "    next value is "  << stackOfParameterRanges.top().second.top() << std::endl;
                ParameterName parameterName = stackOfParameterRanges.top().first;
                nextParameterSet[parameterName] = stackOfParameterRanges.top().second.top();
                stackOfParameterRanges.top().second.pop();
            }
            std::cout << "stack of empty parameter ranges has size " << stackOfEmptyParameterRanges.size() << std::endl;
            // reset each parameter range that has been exhausted
            while ( stackOfEmptyParameterRanges.size() > 0 ) {
                ParameterName parameterName = stackOfEmptyParameterRanges.top().first;
                std::cout << "  reseting range for parameter " << stackOfEmptyParameterRanges.top().first << std::endl;
                stackOfParameterRanges.push(stackOfEmptyParameterRanges.top());
                stackOfEmptyParameterRanges.pop();
                for (ParameterRange::iterator i = parameterRangeMap[parameterName].begin(); i != parameterRangeMap[parameterName].end(); i++ ) {
                    stackOfParameterRanges.top().second.push(*i);
                }
                nextParameterSet[parameterName] = stackOfParameterRanges.top().second.top();
                stackOfParameterRanges.top().second.pop();
            }
            parameterSetVector.push_back(nextParameterSet);
            // print out the next parameter set
            for (ParameterMap::iterator p = nextParameterSet.begin(); p != nextParameterSet.end(); p++) {
                std::cout << "  " << p->first << " : " << p->second << std::endl;
            }
        }
    }
    ~ParameterSetBuilder() {}

    const ParameterMapVector& getParameterSetList() { return parameterSetVector; }

private:
    ParameterMapVector parameterSetVector;
};

class KernelFunction {
public:
    KernelFunction() {}
    virtual ~KernelFunction() {}

    virtual double similarity(const Observation&, const Observation&) = 0;
    virtual void setParameters(const ParameterMap&) = 0;
};


class LinearKernelFunction : KernelFunction {
public:
    LinearKernelFunction() {}
    ~LinearKernelFunction() {}

    double similarity(const Observation& i, const Observation& j) {
        return std::inner_product(i.begin(), i.end(), j.begin(), 0.0);
    }

    void setParameters(const ParameterMap&) {};
};


class RBFKernelFunction : KernelFunction {
public:
    RBFKernelFunction(double g) : gamma(g) {}
    ~RBFKernelFunction() {}

    double similarity(const Observation& i, const Observation& j) {
        double sumOfSquaredDifs = 0.0;
        for (int n = 0; n < i.size(); n++) {
            sumOfSquaredDifs += pow((i[n] - j[n]), 2.0);
        }
        return gamma * exp(sqrt(sumOfSquaredDifs));
    }

    double getGamma()       { return gamma; }
    void setGamma(double g) { gamma = g; }

    void setParameters(const ParameterMap& p) {
        setGamma(p.find("rbf_gamma")->second);
    }

private:
    double gamma;
};


class PolynomialKernelFunction : KernelFunction {
public:
    PolynomialKernelFunction(double _c, int _d) : c(_c), d(_d) {}
    ~PolynomialKernelFunction() {}

    double similarity(const Observation& i, const Observation& j) {
        return pow(std::inner_product(i.begin(), i.end(), j.begin(), c), d);
    }

    void setParameters(const ParameterMap& p) {
        c = p.find("ploynomial_c")->second;
        d = int(p.find("polynomial_d")->second);
    }

private:
    double c;
    int d;
};


class SigmoidKernelFunction : KernelFunction {
public:
    SigmoidKernelFunction(double _alpha, int _c) : alpha(_alpha), c(_c) {}
    ~SigmoidKernelFunction() {}

    double similarity(const Observation& i, const Observation& j) {
        return tanh(alpha * std::inner_product(i.begin(), i.end(), j.begin(), c));
    }

    void setParameters(const ParameterMap& p) {
        alpha = p.find("sigmoid_alpha")->second;
        c = p.find("sigmoid_c")->second;
    }

private:
    double alpha;
    double c;
};


// The SVM class implements the Support Vector Machine
// discriminant function.  Instances are constructed with
// a vector of class labels (+1.0 or -1.0), a vector of dual
// coefficients, a vector of observations, and a bias value.
//
// The class SmoTrainer is responsible for determining the dual
// coefficients and bias value.
//
class SVM {
public:
    SVM(const std::vector<double>& yy, const std::vector<double>& aa, const LabeledObservationVector& oo, double bb, const NumericClassToLabel& mm) :
        y(yy), a(aa), x(oo), b(bb), discriminantToLabel(mm) {}
    ~SVM() {}

    // the classify method should accept a list of observations?
    int discriminant(const Observation& observation);
    Label classify(const Observation& observation);
    double score(const LabeledObservationVector&);

private:
    const std::vector<double> y;
    const std::vector<double> a;
    const LabeledObservationVector x;
    const double b;
    NumericClassToLabel discriminantToLabel; // trouble if this is declared const....
};


// Using SVM with more than two classes requires training multiple SVMs.
// The MultiClassSVM uses a vector of trained SVMs to do classification
// on data having more than two classes.
class MultiClassSVM {
public:
    MultiClassSVM(std::vector<SVM*>);
    ~MultiClassSVM();

    // the classify method should accept a list of observations
    Label classify(const Observation& observation);
    double score(const LabeledObservationVector&);
private:
    const std::vector<SVM*> twoClassSvmList;
};


// SmoTrainer trains a support vector machine using Sequential
// Minimal Optimization as described in the article
// "Support Vector Machine Solvers" by Bottou and Lin.
class SmoTrainer {
public:
    SmoTrainer();
    ~SmoTrainer();

    double getC()       { return C; }
    void setC(double C) { this->C = C; }

    void setParameters(const ParameterMap& p) {
        C = p.find("smo_c")->second;
    }

    SVM* train(const LabeledObservationVector&);
    void assignNumericLabels(std::vector<double>&, const LabeledObservationVector&, NumericClassToLabel&);
    void elementwise_multiply(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c) {
        std::transform(a.begin(), a.end(), b.begin(), c.begin(), std::multiplies<double>());
    }

private:
    double C = 1.0;
};


// OneVsOneMultiClassSvmTrainer trains a support vector machine for each
// pair of labels in a set of data.
class OneVsOneMultiClassSvmTrainer {
public:
    OneVsOneMultiClassSvmTrainer(const LabeledObservationVector& observations);
    ~OneVsOneMultiClassSvmTrainer() {}

    MultiClassSVM* train();
    const LabelSet& getLabelSet() { return labelSet; }
    const LabeledObservationVector& getLabeledObservations() { return labeledObservations; }
    const LabelPairSet& getLabelPairSet() { return labelPairSet; }
    const LabeledObservationVector& getLabeledObservationVectorForLabel(const Label& label) { return labelToLabeledObservationVector[label]; }

    static void buildLabelSet(LabelSet&, const LabeledObservationVector&);
    static void buildLabelToLabeledObservationVector(LabelToLabeledObservationVector&, const LabeledObservationVector&);
    static void buildLabelPairSet(LabelPairSet&, const LabeledObservationVector&);
    static void appendTrainingAndTestingData(Label, const LabeledObservationVector&, LabeledObservationVector&, LabeledObservationVector&);
    static void standardizeObservations(const LabeledObservationVector&);

private:
    const LabeledObservationVector& labeledObservations;
    LabelSet labelSet;
    LabelToLabeledObservationVector labelToLabeledObservationVector;
    LabelPairSet labelPairSet;
};


// KFoldLabeledObservationDivider is used in cross validation to generate
// training and testing data sets of labeled observations.  The labels will
// be distributed in proportion to their frequency in the data, as much as possible.
//
// Consider a data set with 100 observations from five classes.  Also, let
// each class have 20 observations.  If we want to do 10-fold cross validation
// then training sets should have 90 observations and test sets should have
// 10 observations.  A training set should have approximately equal representation
// from each class, as should the test sets.
//
// An instance of KFoldLabeledObservationDivider will generate training and test
// sets within a for loop like this:
//
//   KFoldLabeledObservationDivider X(10, allLabeledObservations);
//   for (X.start(); !X.end(); X.next()) {
//        const LabeledObservationVector& trainingData = X.getTrainingData();
//        const LabeledObservationVector& testingData  = X.getTestingData();
//        // do cross validation on one fold
//   }
class KFoldLabeledObservationsDivider {
public:
    // initialize the k member variable to K so end() will return true if it is called before start()
    // this is not perfect protection against misuse but it's better than nothing
    KFoldLabeledObservationsDivider(int _K, const LabeledObservationVector& l) : K(_K), k(_K) {
        OneVsOneMultiClassSvmTrainer::buildLabelToLabeledObservationVector(labelToLabeledObservationVector, l);
    }
    ~KFoldLabeledObservationsDivider() {}

    // argument labelContainer holds the labels we will work with
    // the trainingData and testingData containers will only have observations with these labels
    // would it be better to restrict the KFoldLabeledObservationsDivider at construction?
    void start(const LabelVector& labelContainer) {
        labelVector.clear();
        labelVector.assign(labelContainer.begin(), labelContainer.end());
        k = 0;
        trainingData.clear();
        testingData.clear();
        for (LabelVector::const_iterator label = labelVector.begin(); label != labelVector.end(); label++) {
            appendKthFold(k, K, labelToLabeledObservationVector[*label], trainingData, testingData);
        }
    }

    bool end() {
        return k >= K;
    }

    void next() {
        k++;
        trainingData.clear();
        testingData.clear();
        for (LabelVector::const_iterator label = labelVector.begin(); label != labelVector.end(); label++) {
            appendKthFold(k, K, labelToLabeledObservationVector[*label], trainingData, testingData);
        }
    }

    int getFoldNumber() { return k; }
    const LabeledObservationVector& getTrainingData() { return trainingData; }
    const LabeledObservationVector& getTestingData() { return testingData; }

    // Function appendKthFold takes care of partitioning the observations in x into two sets,
    // one for training and one for testing.  The argument K specifies how many folds
    // will be requested in all.  The argument k specifies which fold to return.
    // An example: let K=3, k=0, and let there be 10 observations (all having the same label)
    //     i  i%3  (i%3)==0  k=0 partition  (i%3)==1  k=1 partition  (i%3)==2  k=2 partition
    //     0   0     true      testing        false     training       false     training
    //     1   1     false     training       true      testing        false     training
    //     2   2     false     training       false     training       true      testing
    //     3   0     true      testing        false     training       false     training
    //     4   1     false     training       true      testing        false     training
    //     5   2     false     training       false     training       true      testing
    //     6   0     true      testing        false     training       false     training
    //     7   1     false     training       true      testing        false     training
    //     8   2     false     training       false     training       true      testing
    //     9   0     true      testing        false     training       false     training
    //
    static void appendKthFold(int k, int K, const LabeledObservationVector& x, LabeledObservationVector& trainingData, LabeledObservationVector& testingData) {
        for ( int i = 0; i < x.size(); i++) {
            if ( (i % K) == k) {
                testingData.push_back(x[i]);
            }
            else {
                trainingData.push_back(x[i]);
            }
        }
    }

private:
    const int K;
    int k;
    LabelVector labelVector;
    LabelToLabeledObservationVector labelToLabeledObservationVector;
    LabeledObservationVector trainingData;
    LabeledObservationVector testingData;
};

#endif /* svm_hpp_ */
