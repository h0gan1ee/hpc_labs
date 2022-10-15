library("colorspace")

bench <- matrix(
  c(387,  1320, 1747, 236609, 649523,
    5901, 6385, 4908, 55715,  135693,
    421,  386,  3314, 11663,  28569),
  nrow = 5
)
rownames(bench) <- c(
  "(10, 20, 30)",
  "(50, 50, 50)",
  "(1000, 3, 50)",
  "(200, 400, 300)",
  "(800, 100, 800)"
)
colnames(bench) <- c('User', 'OpenMP', 'Apple\'s BLAS')

colors <- darken(hcl.colors(3, palette = "Warm"), 0.1)

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
  ylab = expression(italic('time') / μs),
  xlab = expression(italic('test case')),
  log = 'y',
  xaxt = 'n',
  col = colors
)

axis(1, at = 1:5, labels = rownames(bench))

legend(
  "topleft",
  box.lwd = 0,
  legend = colnames(bench),
  lty = 1:3,
  cex = 0.9,
  y.intersp = 1.31,
  x.intersp = 1.6,
  inset = 0.04,
  col = colors
)
