
PROJ_DIR = $(shell pwd)
RUST_SHIM_DIR = ${PROJ_DIR}/runtime

export

rust-img:
	docker build -t fc-rust-env -f ${RUST_SHIM_DIR}/build.Dockerfile ${RUST_SHIM_DIR}

build-anonymization: rust-img
	${MAKE} -C ${PROJ_DIR}/anonymize-log/func build

build-filter: rust-img
	${MAKE} -C ${PROJ_DIR}/filter-log/func build

deploy-anonymization: build-anonymization
	${MAKE} -C ${PROJ_DIR}/anonymize-log/func deploy

deploy-filter: build-filter
	${MAKE} -C ${PROJ_DIR}/filter-log/func deploy
