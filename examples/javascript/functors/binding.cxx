#include <algorithm>
#include <numeric>
#include <rosetta/generators/js.h>

class DataProcessor : public rosetta::Introspectable {
    INTROSPECTABLE(DataProcessor)
public:
    DataProcessor() { }
    DataProcessor(const std::vector<double>& data)
        : data_(data)
    {
    }

    const std::vector<double>& data() const { return data_; }
    void setData(const std::vector<double>& d) { data_ = d; }

    // C++ → JavaScript
    std::function<double(double)> multiplier(double factor) const
    {
        return [factor](double x) { return x * factor; };
    }

    // JavaScript → C++
    std::vector<double> filter(std::function<bool(double)> predicate) const
    {
        std::vector<double> result;
        std::copy_if(data_.begin(), data_.end(), std::back_inserter(result), predicate);
        return result;
    }

    // JavaScript → C++
    void forEach(std::function<void(double)> callback) const
    {
        std::for_each(data_.begin(), data_.end(), callback);
    }

private:
    std::vector<double> data_;
};

void DataProcessor::registerIntrospection(rosetta::TypeRegistrar<DataProcessor> reg)
{
    reg.constructor<>()
        .constructor<const std::vector<double>&>()
        .method("data", &DataProcessor::data)
        .method("setData", &DataProcessor::setData)
        .method("multiplier", &DataProcessor::multiplier)
        .method("filter", &DataProcessor::filter)
        .method("forEach", &DataProcessor::forEach);
}

// ============================================================================
// Binding
// ============================================================================

BEGIN_JS(generator) {
    registerAllForClasses<DataProcessor>(generator);
}
END_JS()