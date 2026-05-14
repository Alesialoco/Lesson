#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include <sstream>

enum class TypePoint {
    REQUIRED,
    OPTIONAL
};


class Checkpoint {
    std::string name;
    double latitude;
    double longitude;
    TypePoint type;
    double penalty;

public:
    Checkpoint(const std::string& name, double lat, double lon, 
           TypePoint type = TypePoint::REQUIRED, double penal = 0.0) : name(name), type(type){
        if (lat < -90.0 || lat > 90.0) {
            std::cout << "Error: Latitude for checkpoint must be in range [-90.0°, +90.0°]. Setting value to 0.0°" << std::endl;
            latitude = 0.0;
        } else {
            latitude = lat;
        }
        
        if (lon < -180.0 || lon > 180.0) {
            std::cout << "Error: Longitude for checkpoint must be in range [-180.0°, +180.0°]. Setting value to 0.0°" << std::endl;
            longitude = 0.0;
        } else {
            longitude = lon;
        }

        if (type == TypePoint::OPTIONAL) {
            penalty = penal;
        } else {
            penalty = 0.0;
        }
    }

    std::string getName() const { 
        return name; 
    }
    double getLatitude() const { 
        return latitude; 
    }
    double getLongitude() const {
        return longitude; 
    }
    TypePoint getType() const { 
        return type; 
    }
    double getPenalty() const { 
        return penalty; 
    }
    bool isRequired() const { 
        return type == TypePoint::REQUIRED; 
    }
    bool isOptional() const { 
        return type == TypePoint::OPTIONAL; 
    }
};


class Builder_Iface {
public:
    virtual ~Builder_Iface() = default;
    virtual void addCheckpoint(size_t index, const Checkpoint& checkpoint) = 0;
};


class ReportBuilder : public Builder_Iface {
    std::string report;

public:    
    void addCheckpoint(size_t index, const Checkpoint& cp) override {
        std::ostringstream entry;
        entry << index << ". ";
        entry << cp.getName();
        entry << " (" << std::fixed << std::setprecision(4) 
              << cp.getLatitude() << ", " 
              << cp.getLongitude() << ") ";
        
        if (cp.isRequired()) {
            entry << " Незачёт СУ";
        } else {
            entry << "Penalty: " << cp.getPenalty() << " h.";
        }
        
        report += entry.str() + "\n";
    }
    
    std::string getReport() const { 
        return report;
    }
};


class PenaltyCalculatorBuilder : public Builder_Iface {
    double totalPenalty = 0.0;
    std::vector<std::pair<std::string, double>> penalties;
    
public:    
    void addCheckpoint(size_t index, const Checkpoint& cp) override {       
        if (cp.isOptional()) {
            totalPenalty += cp.getPenalty();
            penalties.push_back({cp.getName(), cp.getPenalty()});
            
            std::cout << "КП " << index << " \"" << cp.getName() 
                      << "\": +" << cp.getPenalty() << " h.\n";
        }
    }
    
    double getTotalPenalty() const { 
        return totalPenalty; 
    }
};


void build(Builder_Iface& builder, 
                 const std::vector<Checkpoint>& checkpoints) {
    
    for (size_t i = 0; i < checkpoints.size(); ++i) {
        builder.addCheckpoint(i + 1, checkpoints[i]);
    }
}

int main() {
    std::vector<Checkpoint> checkpoints = {
        Checkpoint("Start", 55.7558, 37.6176, TypePoint::REQUIRED),
        Checkpoint("Ford", 55.7512, 37.6224, TypePoint::OPTIONAL, 2.5),
        Checkpoint("Mountain", 55.7525, 37.6189, TypePoint::OPTIONAL, 1.0),
        Checkpoint("Forest", 55.7496, 37.6098, TypePoint::REQUIRED),
        Checkpoint("River", 55.7601, 37.6312, TypePoint::OPTIONAL, 3.0),
        Checkpoint("Finish", 55.7619, 37.6374, TypePoint::REQUIRED)
    };
    
    ReportBuilder textBuilder;
    build(textBuilder, checkpoints);
    std::cout << textBuilder.getReport() << std::endl;
    
    PenaltyCalculatorBuilder penaltyBuilder;
    build(penaltyBuilder, checkpoints);
    std::cout << "Summary penalty: " << penaltyBuilder.getTotalPenalty() << " hours" << std::endl;
    
    return 0;
}