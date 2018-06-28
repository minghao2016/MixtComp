


#' Aggregate the hierarchical clusters estimated
#'
#' @param dir String, a path to the directory in which are the subclusters to be aggregated
#'
#' @return void
#' @export
#'
#' @examples aggregate_clusters(dir_path_result)
aggregate_clusters <- function(dir) {
  # Si le dossier contient un fichier output
  # Si le warnLog de ce fichier output est vide
  # l'indicatif du cluster est l'indicatif précédent suivi du cluster actuel
  # pour chaque sous cluster contenu dans ce dossier, l'indicatif est le
  # nouvel indicatif suivi de - et de l'indicatif fourni par le cluster suivant
  print(dir)
  output_path = paste0(dir, "/mixtcomp_output.json")
  if (!file.exists(output_path)) {
    return("")
  }
  output = jsonlite::fromJSON(paste0(dir, "/mixtcomp_output.json"))
  if (output$mixture$warnLog != "") {
    return("")
  }
  if (output$mixture$nbInd < 10) {
    return("")
  }
  clusters = output$variable$data$z_class$completed

  existing_subdirs_in_dir = list.dirs(dir, recursive = F)
  if (length(existing_subdirs_in_dir) > 0) {
    for (k in unique(clusters)) {
      path_subdir = paste0(dir, "/subcluster_", k)
      clusters[which(clusters == k)] = paste0(clusters[which(clusters ==
                                                               k)], "-", aggregate_clusters(path_subdir))
    }
  }
  return(clusters)
}



#' Aggregate the hierarchical clusters completed data
#'
#' @param dir String, a path to the directory in which are the subclusters to be aggregated
#' @param var The variable to be aggregated
#'
#' @return void
#' @export
#'
#' @examples aggregate_clusters(dir_path_result)
aggregate_completed <- function(dir, var) {
  # Si le dossier contient un fichier output
  # Si le warnLog de ce fichier output est vide
  # l'indicatif du cluster est l'indicatif précédent suivi du cluster actuel
  # pour chaque sous cluster contenu dans ce dossier, l'indicatif est le
  # nouvel indicatif suivi de - et de l'indicatif fourni par le clsuter suivant
  print(dir)
  output_path = paste0(dir, "/mixtcomp_output.json")
  if (!file.exists(output_path)) {
    return(NA)
  }
  output = jsonlite::fromJSON(paste0(dir, "/mixtcomp_output.json"))

  if (output$mixture$warnLog != "") {
    return(NA)
  }

  clusters = output$variable$data$z_class$completed
  data_completed =  output$variable$data[[var]]$completed

  existing_subdirs_in_dir = list.dirs(dir, recursive = F)
  if (length(existing_subdirs_in_dir) > 0) {
    for (k in unique(clusters)) {
      path_subdir = paste0(dir, "/subcluster_", k)
      idx_cluster = which(clusters == k)
      data_completed_cluster = aggregate_completed(path_subdir, var)
      data_completed[idx_cluster][!is.na(data_completed_cluster)] = data_completed_cluster[!is.na(data_completed_cluster)]
    }
  }
  return(data_completed)
}

#' Plot functional data by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var The name of the functional data to plot
#' @param output The output in which to find the data to plot
#' @param depth maximum depth of the subclustering to plot
#' @param max_nb_lines Defines the maximum number of lines to plot (default is 100)
#'
#' @return a plotly plot
#' @export
#'
#' @examples
aggregate_completed_depth <-
  function(dir,
           var,
           depth) {
    output = fromJSON(paste0(dir, "/mixtcomp_output.json"))
    data_completed = output$variable$data[[var]]$completed
    if (depth == 0) {
      return(data_completed)
    } else {
      clusters = output$variable$data$z_class$completed
      for (cluster in 1:output$mixture$nbCluster) {
        subdir = paste0(dir, "/subcluster_", cluster)
        print(subdir)
        if (file.exists(paste0(subdir, "/mixtcomp_output.json"))) {
          next_output = fromJSON(paste0(subdir, "/mixtcomp_output.json"))
          if (next_output$mixture$warnLog == "") {
            data_completed[which(clusters == cluster)] = aggregate_completed_depth(dir = subdir,
                                                                                   var = var,
                                                                                   depth = depth - 1)
          }
        }
      }
    }
    return(data_completed)
  }



#' Aggregate the classification probabilities estimated in the nested clustering
#'
#' @param clusters vector of the clusters associated with data
#' @param dir String, a path to the directory in which are the subclusters to be aggregated
#' @param depth maximum depth of the subclustering to plot
#'
#' @return a plotly plot
#' @export
#'
#' @examples
#' dir = "data/generated_data"
aggregate_classification_probabilities <-
  function(dir,
           depth) {
    print(dir)

    output = fromJSON(paste0(dir, "/mixtcomp_output.json"))
    probs = output$variable$data$z_class$stat

    if (depth == 0) {
      return(probs)
    } else {
      clusters = output$variable$data$z_class$completed
      probs_completed = c()
      for (cluster in 1:output$mixture$nbCluster) {
        subdir = paste0(dir, "/subcluster_", cluster)
        if (file.exists(paste0(subdir, "/mixtcomp_output.json"))) {
          next_output = fromJSON(paste0(subdir, "/mixtcomp_output.json"))
          if (next_output$mixture$warnLog == "") {
            probs_sub = aggregate_classification_probabilities(subdir, depth = depth - 1)
            probs_all_obs = matrix(0,
                                   ncol = ncol(probs_sub),
                                   nrow = output$mixture$nbInd)
            probs_all_obs[which(cluster == clusters), ] = probs_sub
            probs_completed = cbind(probs_completed, probs_all_obs)
          } else {
            print("WarnLog not empty")
            probs_completed = cbind(probs_completed, probs[, cluster])
          }
        } else {
          print("file doesn't exists")
          probs_completed = cbind(probs_completed, probs[, cluster])
        }
      }
    }
    return(probs_completed)
  }
