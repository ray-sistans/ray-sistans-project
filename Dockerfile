# ---- Build Stage ----
FROM alpine:latest as builder

# Install build dependencies
RUN apk --no-cache add g++ cmake make

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Create build directory and build
RUN mkdir build && cd build && cmake .. && make

# ---- Final Stage ----
FROM alpine:latest

# Install runtime dependencies
RUN apk --no-cache add libgomp libstdc++ libgcc

# Set working directory
WORKDIR /app

# Copy executable from builder stage
COPY --from=builder /app/build/raytracer .

# Copy the output directory
COPY output ./output

# Set the entrypoint
ENTRYPOINT ["./raytracer"]
