FROM atlas/analysistop
ARG CI_JOB_TOKEN
ADD . /Ttres/source/HQTTtResonancesTools
WORKDIR /Ttres/run
RUN source /home/atlas/release_setup.sh && \
    sudo chown -R atlas /Ttres && \
    mkdir /Ttres/build && \
    git clone https://gitlab.cern.ch/atlas-sit/acm.git /Ttres/acm && \
    cp /Ttres/acm/python/templates/new_project_CMakeLists.txt /Ttres/source/CMakeLists.txt && \
    rm -rf /Ttres/acm && \
    cd /Ttres/source && \
    git clone https://gitlab-ci-token:${CI_JOB_TOKEN}@gitlab.cern.ch/atlas-phys/exot/hqt/R21-ttbar-1lep/TtResonancesTools.git && \
    cd /Ttres/build && \
    cmake /Ttres/source && \
    make -j4