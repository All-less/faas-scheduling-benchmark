build: PKG_DIR = ${PROJ_DIR}/anonymize-log/package
build:
	docker run --rm -it \
		-v ${RUST_SHIM_DIR}/bootstrap:/opt/rust-shim \
		-v ${PROJ_DIR}/anonymize-log/code:/opt/function \
		fc-rust-env bash -c "cd /opt/rust-shim && cargo build"
	mkdir -p ${PKG_DIR}
	cp ${RUST_SHIM_DIR}/bootstrap/target/debug/bootstrap ${PKG_DIR}

deploy:
	fun deploy -y
