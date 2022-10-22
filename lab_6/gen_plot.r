library("colorspace")

bench <- matrix(
  c(
    326, 824, 6644, 58305, 463099, 4111110,
    710, 980, 6065, 32598, 269601, 2550128,
    1115, 1129, 3425, 22586, 190886, 1588405,
    603, 797, 3166, 23904, 202312, 1631776,
    277, 41, 270, 1997, 18650, 194895
  ),
  nrow = 6
)

rownames(bench) <- c(
  "8",
  "64",
  "128",
  "256",
  "512",
  "1024"
)
colnames(bench) <- c("Naive", "OpenMPI", "pthread", "OpenMP", "Apple's BLAS")

colors <- darken(hcl.colors(5, palette = "Warm"), 0.1)

par(
  family = "CMU Serif",
  mar = c(4.5, 5, 3.5, 4),
  cex.axis = 0.9,
  cex = 1
)

p <- matplot(
  bench,
  main = "DGEMM Benchmark",
  type = "l",
  ylab = expression(italic("time") / μs),
  xlab = expression(italic("dimension size")),
  ylim = c(60, 3e6),
  log = "y",
  xaxt = "n",
  col = colors
)

axis(1, at = 1:6, labels = rownames(bench))

legend(
  "topleft",
  box.lwd = 0,
  legend = colnames(bench),
  lty = 1:5,
  cex = 0.9,
  y.intersp = 1.31,
  x.intersp = 1.6,
  inset = 0.04,
  col = colors
)
