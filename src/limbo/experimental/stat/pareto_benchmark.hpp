//| Copyright Inria May 2015
//| This project has received funding from the European Research Council (ERC) under
//| the European Union's Horizon 2020 research and innovation programme (grant
//| agreement No 637972) - see http://www.resibots.eu
//| 
//| Contributor(s):
//|   - Jean-Baptiste Mouret (jean-baptiste.mouret@inria.fr)
//|   - Antoine Cully (antoinecully@gmail.com)
//|   - Kontantinos Chatzilygeroudis (konstantinos.chatzilygeroudis@inria.fr)
//|   - Federico Allocati (fede.allocati@gmail.com)
//|   - Vaios Papaspyros (b.papaspyros@gmail.com)
//| 
//| This software is a computer library whose purpose is to optimize continuous,
//| black-box functions. It mainly implements Gaussian processes and Bayesian
//| optimization.
//| Main repository: http://github.com/resibots/limbo
//| Documentation: http://www.resibots.eu/limbo
//| 
//| This software is governed by the CeCILL-C license under French law and
//| abiding by the rules of distribution of free software.  You can  use,
//| modify and/ or redistribute the software under the terms of the CeCILL-C
//| license as circulated by CEA, CNRS and INRIA at the following URL
//| "http://www.cecill.info".
//| 
//| As a counterpart to the access to the source code and  rights to copy,
//| modify and redistribute granted by the license, users are provided only
//| with a limited warranty  and the software's author,  the holder of the
//| economic rights,  and the successive licensors  have only  limited
//| liability.
//| 
//| In this respect, the user's attention is drawn to the risks associated
//| with loading,  using,  modifying and/or developing or reproducing the
//| software by the user in light of its specific status of free software,
//| that may mean  that it is complicated to manipulate,  and  that  also
//| therefore means  that it is reserved for developers  and  experienced
//| professionals having in-depth computer knowledge. Users are therefore
//| encouraged to load and test the software's suitability as regards their
//| requirements in conditions enabling the security of their systems and/or
//| data to be ensured and,  more generally, to use and operate it in the
//| same conditions as regards security.
//| 
//| The fact that you are presently reading this means that you have had
//| knowledge of the CeCILL-C license and that you accept its terms.
//| 
#ifndef LIMBO_STAT_PARETO_BENCHMARK_HPP
#define LIMBO_STAT_PARETO_BENCHMARK_HPP

#include <limbo/limbo.hpp>

namespace limbo {
    namespace experimental {
        namespace stat {
            template <typename F>
            struct ParetoBenchmark {
                template <typename BO, typename AggregatorFunction>
                void operator()(BO& opt, const AggregatorFunction& afun, bool blacklisted)
                {
                    opt.update_pareto_data();
#ifndef NSBO // this is already done is NSBO
                    opt.template update_pareto_model<F::dim_in>();
#endif
                    auto dir = opt.res_dir() + "/";
                    auto p_model = opt.pareto_model();
                    auto p_data = opt.pareto_data();
                    std::string it = std::to_string(opt.current_iteration());
                    std::string model = dir + "pareto_model_" + it + ".dat";
                    std::string model_real = dir + "pareto_model_real_" + it + ".dat";
                    std::string data = dir + "pareto_data_" + it + ".dat";
                    std::string obs_f = dir + "obs_" + it + ".dat";
                    std::ofstream pareto_model(model.c_str()), pareto_data(data.c_str()),
                        pareto_model_real(model_real.c_str()), obs(obs_f.c_str());
                    F f;
                    for (auto x : p_model)
                        pareto_model << std::get<1>(x).transpose() << " "
                                     << std::get<2>(x).transpose() << std::endl;
                    for (auto x : p_model)
                        pareto_model_real << f(std::get<0>(x)).transpose() << " " << std::endl;
                    for (auto x : p_data)
                        pareto_data << std::get<1>(x).transpose() << std::endl;
                    for (size_t i = 0; i < opt.observations().size(); ++i)
                        obs << opt.observations()[i].transpose() << " "
                            << opt.samples()[i].transpose() << std::endl;
                }
            };
        }
    }
}

#endif