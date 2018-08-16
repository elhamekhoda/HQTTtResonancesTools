ARG gitlab_ci_token
ARG AtlasVersion
FROM atlas/analysistop:$AtlasVersion
ADD . /Ttres/source/HQTTtResonancesTools
WORKDIR /Ttres/run
RUN source /home/atlas/release_setup.sh && \
    sudo chown -R atlas /Ttres && \
    mkdir /Ttres/build && \
    git clone https://gitlab.cern.ch/atlas-sit/acm.git /Ttres/acm && \
    cp /Ttres/acm/python/templates/new_project_CMakeLists.txt /Ttres/source/CMakeLists.txt && \
    rm -rf /Ttres/acm && \
    cd /Ttres/source && \
    echo ${gitlab_ci_token} && \
    git clone https://gitlab-ci-token:${gitlab_ci_token}@gitlab.cern.ch/elham/BoostedJetTaggers.git && \
    git clone https://gitlab-ci-token:${gitlab_ci_token}@gitlab.cern.ch/atlas-phys/exot/hqt/R21-ttbar-1lep/TtResonancesTools.git && \
    cd /Ttres/build && cmake /Ttres/source && \
    make -j4